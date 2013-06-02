#pragma once
#include <ros/ros.h>
#include <boost/shared_ptr.hpp>
#include "SpeechRecognitionClient.h"
#include "UserRecognitionClient.h"
#include <std_msgs/String.h>
#include <RDP/DetectMotion.h>

class Unity{
    boost::shared_ptr<nui::SpeechRecognitionClient> m_speechRecog;
    boost::shared_ptr<nui::UserRecognitionClient> m_userRecog;
    boost::shared_ptr<ros::NodeHandle> m_node;
    //ros::Subscriber m_retSub;
    void speechRetCb(const std_msgs::StringConstPtr &ret);
    void detectMotionCb(const RDP::DetectMotionConstPtr &motion);

public:
    Unity(int argc, char *argv[]);
    void startSpeechRecognition();
    void statusUpdate();

};

