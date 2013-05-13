/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file SpeechRecognitionServer.h
 * @brief Speech Recognition
 * @author Daiki Maekawa
 * @date 2012-12-28
 *******************************************************************/

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <string>
#include <vector>
#include "Timer.h" 

typedef struct __Recog__ Recog;

namespace nui{

    class SpeechRecognitionServer{
        atlas::Timer m_timer;
        boost::shared_ptr<Recog> m_recog;
		ros::NodeHandle &m_node;
		ros::Publisher  m_resultPub;
		ros::Subscriber m_startSub;
		ros::Subscriber m_pauseSub;
		ros::Subscriber m_finishSub;
        bool m_isValid;
        bool m_isOpenStream;
        bool m_isPausedStream;
        bool initJulius(int argc, char *argv[]);
		std::string stringConvert(const std::string &src,const char *srcEnc,const char *retEnc);
        static void callWhenResult(Recog *recog, void *receiver);
        static void callInPause(Recog *recog, void *receiver);
        static void callWhenResume(Recog *recog, void *receiver);

    public:
        SpeechRecognitionServer(int argc, char *argv[], ros::NodeHandle &node); 
        bool isValid() const{ return m_isValid; }
        void startRecognition(const std_msgs::EmptyConstPtr &msg);
        void finishRecognition(const std_msgs::EmptyConstPtr &msg);
        void pauseRecognition(const std_msgs::EmptyConstPtr &msg);
		void runServer();
    };

}; //namespace nui

