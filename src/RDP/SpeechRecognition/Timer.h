/*******************************************************************
 * Copyright (c) 2011 Daiki Maekawa
 *
 * @file Timer.h
 * @brief Used as a simple timer
 * @author Daiki Maekawa
 * @date 2011-10-19
 *******************************************************************/

#pragma once
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <boost/thread.hpp>

namespace atlas{

    class Timer{
        boost::mutex m_mutex;
        boost::condition_variable_any m_exitRequest;
        volatile bool m_endFlag;
        void roop(unsigned long cycle, boost::function<void()> callback, unsigned int callbackNum = 0);
    public:
        Timer() : m_endFlag(false) { }
        ~Timer();
        static void sleep(unsigned long sec);
        void singleShot(unsigned long sec, boost::function<void()> callback);
        void start(unsigned long cycle, boost::function<void()>); //cycle[sec]
        void stop();
    };

} //namespace atlas

