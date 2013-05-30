/*******************************************************************
 * Copyright (c) 2013 Daiki Maekawa
 *
 * @file MotionDetector.h
 * @brief Motion detection of user
 * @author Daiki Maekawa
 * @date 2013-04-18
 *******************************************************************/

#pragma once

#include "Motion.h"
//#include "PoseManager.h"
#include <vector>
#include <string>
#include <boost/function.hpp>
#include <boost/functional.hpp>
#include <boost/shared_ptr.hpp>
#include <RDP/UserStatus.h>
//#include <RDP/DetectMotion.h>

namespace nui{

typedef boost::function<void(int)> DetectMotion;

class MotionDetector{
    std::vector<Motion> m_motions;
    //std::vector<PoseManager> m_motions;
    DetectMotion m_detectCb;

public:
    MotionDetector();
    void loadFromMotionFiles(const std::string &dirpath);
    void setDetectCb(DetectMotion callback){ m_detectCb = callback; }
    void updateUsers(const std::vector<RDP::UserStatus> &users, unsigned int elapsedTime); //elapsedTime[sec]
    bool isDetectedPose(Motion &target, const RDP::UserStatus &user, unsigned int elapsedTime);
    //bool isDetectedPose(PoseManager &target, const RDP::UserStatus &user, unsigned int elapsedTime);

};

} //namespace nui

