#pragma once
#include <ros/ros.h>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

namespace nui{
    
    static const char * const START_SPEECH_RECOGNITION = "StartSpeechRecognition";
    static const char * const RESULT_SPEECH_RECOGNITION = "ResultSpeechRecognition";
    static const char * const PAUSE_SPEECH_RECOGNITION = "PauseSpeechRecognition";
    static const char * const FINISH_SPEECH_RECOGNITION = "FinishSpeechRecognition";

    class SpeechRecognitionClient{
        ros::NodeHandle &m_node;
		ros::Subscriber m_retsultSub;
		ros::Publisher  m_startPub;
		ros::Publisher  m_pausePub;
		ros::Publisher  m_finishPub;

    public:
        SpeechRecognitionClient(ros::NodeHandle &node);
		void setResultCallback(boost::function<void(const std_msgs::StringConstPtr&)> callback){
			m_retsultSub = m_node.subscribe(RESULT_SPEECH_RECOGNITION, 100, callback);
		}
        void startRecognition(){ 
			ROS_INFO("publish startRecognition");
			std_msgs::Empty msg;
			m_startPub.publish(msg); 
		}

		void pauseRecognition(){
			ROS_INFO("publish pauseRecognition");
			std_msgs::Empty msg;
		    m_pausePub.publish(msg);
		}

		void finishRecognition(){
			ROS_INFO("publish finishRecognition");
			std_msgs::Empty msg;
			m_finishPub.publish(msg);
		}
    };
};
