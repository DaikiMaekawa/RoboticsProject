#include "Timer.h"
#include <iostream>

namespace atlas{

    void Timer::roop(unsigned long cycle, boost::function<void()> callback, unsigned int callbackNum){
        unsigned int callbackCount = 0;

        while(true){
            if(m_endFlag) break;
            if(callbackNum != 0 && callbackCount >= callbackNum) break;
            
            boost::this_thread::sleep(boost::posix_time::seconds(cycle));
            callback();
            if(callbackNum != 0) callbackCount++;
        }
    }

    void Timer::sleep(unsigned long sec){
        boost::this_thread::sleep(boost::posix_time::seconds(sec));
    }

    void Timer::singleShot(unsigned long sec, boost::function<void()> callback){
        m_endFlag = false;
        boost::thread thread(boost::bind(&Timer::roop, this, sec, callback, 1));
    }

    void Timer::start(unsigned long cycle, boost::function<void()> callback){
        m_endFlag = false;
        boost::thread thread(boost::bind(&Timer::roop, this, cycle, callback, 0));
    }

    void Timer::stop(){
        m_endFlag = true;
    }

    Timer::~Timer(){
        stop();
    }

} //namespace atlas
