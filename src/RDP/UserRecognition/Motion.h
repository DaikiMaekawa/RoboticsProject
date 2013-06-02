#pragma once

#include <string>
#include <RDP/UserPose.h>
#include <vector>
#include <map>

namespace nui{

class Motion
{
    std::vector<RDP::UserPose> m_poses;
    unsigned int m_motionId;
    float m_detectTime;
    unsigned int m_detectingPoseId;
    bool m_allPosesIsDetected;

public:
    static const int MAX_JOINT_NUM = 15;
    Motion();
    bool saveAs(const std::string &file);
    bool loadFrom(const std::string &file);
    bool poseIsExist(unsigned int poseId);
    //bool setPose(unsigned int poseId, const RDP::UserPose &pose);
    bool setPose(const RDP::UserPose &pose);
    //bool pose(unsigned int poseId, RDP::UserPose &pose);
    const RDP::UserPose& pose(unsigned int poseId);
    bool addPose(const RDP::UserPose &pose);
    bool erasePose(unsigned int poseId);
    void update(float elapsedTime, bool isDetectedPose);
    const RDP::UserPose& shouldDetectPose();
    unsigned int id(){ return m_motionId; }
    bool allPosesIsDetected(); 

};

} //namespace nui

