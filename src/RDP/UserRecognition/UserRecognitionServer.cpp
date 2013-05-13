#include "UserRecognitionServer.h"
#include "UserRecognitionClient.h"
#include <XnCppWrapper.h>
#include "UserDetector.h"
#include "Xtion.h"
#include "XnErrorChecker.h"
#include <cv_bridge/cv_bridge.h>
//#include <image_transport/image_transport.h>
#include <string>

namespace nui{

    class UserRecognitionServer::Impl{
        xn::Context m_context;
        boost::shared_ptr<Xtion>           m_device;
        boost::shared_ptr<UserDetector>    m_userDetector;
		ros::Subscriber m_startSub;
		ros::Publisher m_rgbPub;
		ros::Publisher m_depthPub;

    public:
        Impl(ros::NodeHandle &node) : 
			m_startSub(node.subscribe(START_USER_RECOGNITION, 100, &Impl::startRecognition, this)),
			m_rgbPub(node.advertise<cv_bridge::CvImage>(RGB_IMAGE, 100)),
			m_depthPub(node.advertise<cv_bridge::CvImage>(DEPTH_IMAGE, 100))
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
    	
        void startRecognition(const std_msgs::EmptyConstPtr &msg){
            ROS_INFO("receive startRecognition");
			try{
				xnErrorCheck(m_context.StartGeneratingAll());
			}catch(std::exception &ex){
				std::cout << ex.what() << std::endl;
			}
        }

        void waitUpdateAll(){
            m_context.WaitAndUpdateAll();
        }

        cv_bridge::CvImagePtr depthImage(){
            cv_bridge::CvImagePtr depth;
			depth->image = *m_device->depthImage();
			return depth;
        }
        
        cv_bridge::CvImagePtr rgbImage(){
		    cv_bridge::CvImagePtr rgb;
            rgb->image = *m_device->rgbImage();
			return rgb;
        }

        NIMat userDepthImage(const UserStatus &user){
            return m_userDetector->userDepthMetaData(user);
        }

        const std::vector<UserStatus>& detectUsers(){
            m_userDetector->updateAllUserStatus();
            return m_userDetector->detectUsers();
        }
    	
        bool isValid() const{
            return m_userDetector->isValid() && m_device->isValid();
        }

		void runServer(){
			ros::Rate loopRate(5);
			while(ros::ok()){
				waitUpdateAll();
				ros::spinOnce();
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

    const std::vector<UserStatus>& UserRecognitionServer::detectUsers(){
        return pImpl->detectUsers();
    }

    bool UserRecognitionServer::isValid() const{
        return pImpl->isValid();
    }

    void UserRecognitionServer::runServer(){
        pImpl->runServer();
    }

} //namespace nui
