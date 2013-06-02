#include "UserRecognitionServer.h" 
#include "UserRecognitionClient.h"
#include <XnCppWrapper.h>
#include "UserDetector.h"
#include "MotionDetector.h"
#include "Xtion.h"
#include "XnErrorChecker.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <string>
#include <iostream>
#include <RDP/UserStatus.h>
#include <RDP/DetectUsers.h>
#include <RDP/DetectMotion.h>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

namespace nui{

    static const XnSkeletonJoint UseJointTable[] = {
		XN_SKEL_HEAD,
		XN_SKEL_NECK,
		XN_SKEL_TORSO,
		XN_SKEL_LEFT_ELBOW,
		XN_SKEL_RIGHT_ELBOW,
		XN_SKEL_LEFT_HIP,
		XN_SKEL_RIGHT_HIP,
		XN_SKEL_LEFT_KNEE,
		XN_SKEL_RIGHT_KNEE,
		XN_SKEL_LEFT_HAND,
		XN_SKEL_RIGHT_HAND,
		XN_SKEL_LEFT_SHOULDER,
		XN_SKEL_RIGHT_SHOULDER,
		XN_SKEL_LEFT_FOOT,
		XN_SKEL_RIGHT_FOOT
    };

    static const int JointTableSize = 15;
    static const float PERIOD = 0.5f;    //sec

    class UserRecognitionServer::Impl{
        xn::Context    m_context;
        MotionDetector m_motionDetector;
        boost::shared_ptr<Xtion>           m_device;
        boost::shared_ptr<UserDetector>    m_userDetector;
        ros::Publisher m_rgbPub;
        ros::Publisher m_depthPub;
        ros::Publisher m_detectUsersPub;
        ros::Publisher m_motionPub;

    public:
        Impl(ros::NodeHandle &node) :
            m_rgbPub(node.advertise<sensor_msgs::Image>(RGB_IMAGE, 100)),
            m_depthPub(node.advertise<sensor_msgs::Image>(DEPTH_IMAGE, 100)),
            m_detectUsersPub(node.advertise<RDP::DetectUsers>(DETECT_USERS, 100)),
            m_motionPub(node.advertise<RDP::DetectMotion>(DETECT_MOTION, 100))
        {
            try{
                m_motionDetector.setDetectCb(boost::bind(&UserRecognitionServer::Impl::detectMotionCb, this, boost::lambda::_1));
                m_motionDetector.loadFromMotionFiles(std::string(SHARE_DIR) + "/motions");
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

        void detectMotionCb(int id){
            std::cout << "detectMotionCb(" << id << ")" << std::endl;
            RDP::DetectMotion msg;
            msg.id = id;
            m_motionPub.publish(msg);
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
            m_userDetector->updateAllUserStatus();
        }

        void sendDepthImage(){
            std::vector<UserStatus> status = detectUsers();
            if(status.size() > 0){
                NIMat image = m_userDetector->userDepthMetaData(status[0]);
                if(image){
                    cv_bridge::CvImage depth;
                    depth.image = *image;
                    m_depthPub.publish(depth.toImageMsg());
                }else{
                    std::cout << "Failed sendDepthImage" << std::endl;
                }
            }

            /*
			NIMat image = m_device->depthImage();
			if(image){
            	cv_bridge::CvImage depth;
				depth.image = *image;
            	m_depthPub.publish(depth.toImageMsg());
			}else{
				std::cout << "Failed sendDepthImage" << std::endl;
			}
            */
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
		
		/*
        NIMat userDepthImage(const UserStatus &user){
            NIMat image = m_device->userDepthMetaData(user)
			
			return m_userDetector->userDepthMetaData(user);
        }
		*/
		
		
        const std::vector<UserStatus>& detectUsers(){
            m_userDetector->updateAllUserStatus();
            return m_userDetector->detectUsers();
        }

		void sendDetectUsers(){
			RDP::DetectUsers users;
			std::vector<UserStatus> status = detectUsers();

			for(int i=0; i < status.size(); i++){
				if(status[i].isTracking){
                    users.data.push_back(RDP::UserStatus());
                    users.data[i].id = i;
                    users.data[i].isTracking = status[i].isTracking;
                    
                    for(int m=0; m < JointTableSize; m++){
                        users.data[i].pose.joints.push_back(RDP::UserJoint());
                        users.data[i].pose.joints[m].type =  status[i].joint(UseJointTable[m]).jointType;
                        users.data[i].pose.joints[m].pos.x = status[i].joint(UseJointTable[m]).x;
                        users.data[i].pose.joints[m].pos.y = status[i].joint(UseJointTable[m]).y;	
                        users.data[i].pose.joints[m].pos.z = status[i].joint(UseJointTable[m]).z;	
                    }
                }
			}
            m_motionDetector.updateUsers(users.data, PERIOD);
			m_detectUsersPub.publish(users);
		}

        bool isValid() const{
            return m_userDetector->isValid() && m_device->isValid();
        }

        void runServer(){
			startRecognition();
			ros::Rate loopRate(1.f/PERIOD);
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

    bool UserRecognitionServer::isValid() const{
        return pImpl->isValid();
    }

    void UserRecognitionServer::runServer(){
        pImpl->runServer();
    }

} //namespace nui
