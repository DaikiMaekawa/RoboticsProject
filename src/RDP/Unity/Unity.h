#pragma once
#include <ros/ros.h>
#include <boost/shared_ptr.hpp>
#include "SpeechRecognitionClient.h"
#include "UserRecognitionClient.h"
#include "CommandSenderClient.h"
#include <std_msgs/String.h>
#include <RDP/DetectMotion.h>
#include <string>
#include <vector>

class Unity{
    boost::shared_ptr<nui::SpeechRecognitionClient> m_speechRecog;
    boost::shared_ptr<nui::UserRecognitionClient> m_userRecog;
    boost::shared_ptr<spp::CommandSenderClient> m_cmd;
    boost::shared_ptr<ros::NodeHandle> m_node;
    std::vector<unsigned int> m_detectMotionId;
    void speechRetCb(const std_msgs::StringConstPtr &ret);
    void detectMotionCb(const RDP::DetectMotionConstPtr &motion);

public:
    Unity(int argc, char *argv[]);
    void startSpeechRecognition();
    void lcdPuts(const std::string &str);
    void lcdClear();
    void motor(int rDuty, int lDuty);
    void statusUpdate();
    std::vector<unsigned int> detectMotionId(){ 
        std::vector<unsigned int> detectMotionId = m_detectMotionId;
        m_detectMotionId.clear();
        return detectMotionId; 
    }

};

