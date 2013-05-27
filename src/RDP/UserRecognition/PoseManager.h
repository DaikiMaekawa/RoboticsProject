#pragma once

#include "Motion.h"
#include <RDP/UserPose.h>

namespace nui{

class PoseManager{
    Motion m_motion;
    RDP::UserPose m_detectingPose;
    unsigned int m_detectTime; //sec

public:
    explicit PoseManager(const Motion &motion);
    const RDP::UserPose& shouldDetectPose(unsigned int elapsedTime);

};

} //namespace nui

