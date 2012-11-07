#pragma once

#include "Motion.h"
#include <RDP/UserPose.h>

namespace nui{

class PoseManager{
    Motion m_motion;
    RDP::UserPose m_detectingPose;
    unsigned int m_detectTime; //sec
    unsigned int m_poseId;
    bool         m_isValid;
    void setShouldDetectPose();

public:
    explicit PoseManager(const Motion &motion);
    bool shouldDetectPose(unsigned int elapsedTime, RDP::UserPose &pose);
    unsigned motionId() { return m_motion.id(); }

};

} //namespace nui

