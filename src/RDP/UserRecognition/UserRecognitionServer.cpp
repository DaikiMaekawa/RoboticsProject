#include "UserRecognitionServer.h" 
#include "UserRecognitionClient.h"
#include <XnCppWrapper.h>
#include "UserDetector.h"
#include "Xtion.h"
#include "XnErrorChecker.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <string>
#include <iostream>
#include <RDP/UserStatus.h>
#include <RDP/DetectUsers.h>

namespace nui{

    class UserRecognitionServer::Impl{
        xn::Context m_context;
        boost::shared_ptr<Xtion>           m_device;
        boost::shared_ptr<UserDetector>    m_userDetector;
		ros::Publisher m_rgbPub;
		ros::Publisher m_depthPub;
		ros::Publisher m_detectUsersPub;

    public:
        Impl(ros::NodeHandle &node) : 
			m_rgbPub(node.advertise<sensor_msgs::Image>(RGB_IMAGE, 100)),
			m_depthPub(node.advertise<sensor_msgs::Image>(DEPTH_IMAGE, 100)),
			m_detectUsersPub(node.advertise<RDP::DetectUsers>(DETECT_USERS, 100))
		{
            try{
				std::string initFile = std::string(SHARE_DIR) + "/InitDevice.xml";
				ROS_INFO("UserRecognition Initfile = %s", initFile.c_str());
                xnErrorCheck(m_context.InitFromXmlFile(initFile.c_str()));
                xnErrorCheck(xnPrintRegisteredLicenses());
                m_context.SetGlobalMirror(true);
                
                m_device = boost::shared_ptr<Xtion>(new Xtion(m_context));
                m_userDetector = boost::shared_ptr<UserDetector>(new UserDetector(*m_device,m_context));
            }catch(std::exception &ex){
                std::cout << ex.what() << std::endl;
            }
        }
    	
        void startRecognition(){
            ROS_INFO("start UserRecognition");
			try{
				xnErrorCheck(m_context.StartGeneratingAll());
			}catch(std::exception &ex){
				std::cout << ex.what() << std::endl;
			}
        }

        void waitUpdateAll(){
            m_context.WaitAndUpdateAll();
        }

        void sendDepthImage(){
			NIMat image = m_device->depthImage();
			if(image){
            	cv_bridge::CvImage depth;
				depth.image = *image;
            	m_depthPub.publish(depth.toImageMsg());
			}else{
				std::cout << "Failed sendDepthImage" << std::endl;
			}
		}
        
        void sendRgbImage(){
            NIMat image = m_device->rgbImage();
            if(image){
		    	cv_bridge::CvImage rgb;
				rgb.image = *image;
				m_rgbPub.publish(rgb.toImageMsg()); 
			}else{
				std::cout << "Failed sendRgbImage" << std::endl;
			}
		}

        NIMat userDepthImage(const UserStatus &user){
            return m_userDetector->userDepthMetaData(user);
        }
		
		/*
        const std::vector<UserStatus>& detectUsers(){
            m_userDetector->updateAllUserStatus();
            return m_userDetector->detectUsers();
        }
		*/

		void sendDetectUsers(){
			RDP::DetectUsers users;
			m_userDetector->updateAllUserStatus();
			std::vector<UserStatus> status = m_userDetector->detectUsers();
			for(int i=0; i < status.size(); i++){
				users.data.push_back(RDP::UserStatus());
				users.data[i].id = i;
				users.data[i].isTracking = status[i].isTracking;
				
				for(int m=0; m < UserStatus::MAX_JOINT_NUM; m++){
					users.data[i].joints.push_back(RDP::UserJoint());
					users.data[i].joints[m].type =  status[i].joint((XnSkeletonJoint)(m+1)).jointType;
					users.data[i].joints[m].pos.x = status[i].joint((XnSkeletonJoint)(m+1)).x;
					users.data[i].joints[m].pos.y = status[i].joint((XnSkeletonJoint)(m+1)).y;	
					users.data[i].joints[m].pos.z = status[i].joint((XnSkeletonJoint)(m+1)).z;	
				}
			}
			m_detectUsersPub.publish(users);
		}
    	
        bool isValid() const{
            return m_userDetector->isValid() && m_device->isValid();
        }

		void runServer(){
			startRecognition();
			ros::Rate loopRate(5);
			while(ros::ok()){
				waitUpdateAll();
				ros::spinOnce();
				sendRgbImage();
				sendDepthImage();
				sendDetectUsers();
				loopRate.sleep();
			}
		}

        ~Impl(){
            m_context.Shutdown();
        }

    };

    UserRecognitionServer::UserRecognitionServer(ros::NodeHandle &node) : pImpl(new Impl(node))
    {

    }

    NIMat UserRecognitionServer::userDepthImage(const UserStatus &user){
        return pImpl->userDepthImage(user);
    }

    bool UserRecognitionServer::isValid() const{
        return pImpl->isValid();
    }

    void UserRecognitionServer::runServer(){
        pImpl->runServer();
    }

} //namespace nui
