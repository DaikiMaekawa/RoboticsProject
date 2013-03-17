/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file SpeechRecognition.h
 * @brief Speech Recognition
 * @author Daiki Maekawa
 * @date 2012-12-28
 *******************************************************************/

#pragma once

extern "C"{ 
    #include <julius/juliuslib.h> 
}
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>
#include "Atlas/Timer.h"

namespace nui{

    class SpeechRecognition{
        atlas::Timer m_timer;
        boost::shared_ptr<Recog> m_recog;
        bool m_isValid;
        bool m_isOpenStream;
        bool m_isPausedStream;
        std::vector<std::string> m_resultsString;
        bool initJulius(int argc, char *argv[]);
        static void callWhenResult(Recog *recog, void *receiver);
        static void callInPause(Recog *recog, void *receiver);
        static void callWhenResume(Recog *recog, void *receiver);
    public:
        SpeechRecognition(int argc, char *argv[]); 
        bool isValid() const{ return m_isValid; }
        bool startRecognition(unsigned long timeout);
        void finishRecognition();
        void pauseRecognition();
        std::vector<std::string> resultsString(){
            std::vector<std::string> ins = m_resultsString;
            m_resultsString.clear();
            return ins;
        }
    };

} //namespace nui

