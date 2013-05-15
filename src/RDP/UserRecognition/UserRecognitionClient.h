#pragma once
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Empty.h>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <RDP/DetectUsers.h>

namespace nui{
    
    //static const char * const START_USER_RECOGNITION = "StartUserRecognition";
    static const char * const DETECT_USERS           = "DetectUsers";
	static const char * const RGB_IMAGE              = "RGBImage";
	static const char * const DEPTH_IMAGE            = "DepthImage";
	
    class UserRecognitionClient{
        ros::NodeHandle &m_node;
		ros::Subscriber  m_rgbSub;
		ros::Subscriber  m_depthSub;
		ros::Subscriber  m_detectUsersSub;

    public:
        UserRecognitionClient(ros::NodeHandle &node);
		void setRGBCb(boost::function<void(const sensor_msgs::ImageConstPtr&)> callback){
			m_rgbSub = m_node.subscribe(RGB_IMAGE, 100, callback);
		}
		
		void setDepthCb(boost::function<void(const sensor_msgs::ImageConstPtr&)> callback){
			m_depthSub = m_node.subscribe(DEPTH_IMAGE, 100, callback);
		}

		void setDetectUsersCb(boost::function<void(const RDP::DetectUsersConstPtr&)> callback){
			m_detectUsersSub = m_node.subscribe(DETECT_USERS, 100, callback);
		}

    };
}; //namespace nui
