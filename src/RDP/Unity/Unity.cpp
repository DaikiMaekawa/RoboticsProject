#include "Unity.h"
#include <std_msgs/Empty.h>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

Unity::Unity(int argc, char *argv[]){
    ros::init(argc, argv, "Unity");
    
    m_node = boost::shared_ptr<ros::NodeHandle>(new ros::NodeHandle());
    m_speechRecog = boost::shared_ptr<nui::SpeechRecognitionClient>(new nui::SpeechRecognitionClient(*m_node));
    m_speechRecog->setResultCallback(boost::bind(&Unity::speechRetCallback, this, boost::lambda::_1));
    
    m_userRecog = boost::shared_ptr<nui::UserRecognitionClient>(new nui::UserRecognitionClient(*m_node));

}

void Unity::speechRetCallback(const std_msgs::StringConstPtr &ret){
    std::cout << "speechRetCallback = " << ret->data << std::endl;  
}

void Unity::startSpeechRecognition(){
    m_speechRecog->startRecognition();
    /*
    ros::Rate loopRate(5);
    ros::Publisher publisher = m_node->advertise<std_msgs::Empty>("startSpeechRecognition", 100);
    for(int i=0; i < 3; i++) loopRate.sleep();
    
    std_msgs::Empty msg;
    publisher.publish(msg);
    ROS_INFO("publish startSpeechRecognition");
    ros::spinOnce();
    loopRate.sleep();
    */
}

void Unity::startUserRecognition(){
    m_userRecog->startRecognition();
}

void Unity::statusUpdate(){
    ros::spinOnce();
    sleep(1);
}

