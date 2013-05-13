#pragma once
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <cv_bridge/cv_bridge.h>

namespace nui{
    
    static const char * const START_USER_RECOGNITION = "StartUserRecognition";
    static const char * const RGB_IMAGE              = "RGBImage";
	static const char * const DEPTH_IMAGE            = "DepthImage";
	
    class UserRecognitionClient{
        ros::NodeHandle &m_node;
		ros::Publisher   m_startPub;
		ros::Subscriber  m_rgbSub;
		ros::Subscriber  m_depthSub;

    public:
        UserRecognitionClient(ros::NodeHandle &node);
		void setRGBCb(boost::function<void(const cv_bridge::CvImageConstPtr&)> callback){
			m_rgbSub = m_node.subscribe(RGB_IMAGE, 100, callback);
		}
		
		void setDepthCb(boost::function<void(const cv_bridge::CvImageConstPtr&)> callback){
			m_depthSub = m_node.subscribe(DEPTH_IMAGE, 100, callback);
		}

        void startRecognition(){
            ROS_INFO("publish startRecognition");
			std_msgs::Empty msg;
			m_startPub.publish(msg);
		}
    };
}; //namespace nui
