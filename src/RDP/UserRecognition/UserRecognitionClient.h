#pragma once
#include <ros/ros.h>
#include <std_msgs/Empty.h>

namespace nui{
    
    static const char * const START_USER_RECOGNITION = "StartUserRecognition";
    static const char * const IMAGE                  = "Image";

    class UserRecognitionClient{
        ros::NodeHandle &m_node;
	ros::Publisher m_startPub;

    public:
        UserRecognitionClient(ros::NodeHandle &node);
        void startRecognition(){
            ROS_INFO("publish startRecognition");
	    std_msgs::Empty msg;
	    m_startPub.publish(msg);
	}
    };
}; //namespace nui
