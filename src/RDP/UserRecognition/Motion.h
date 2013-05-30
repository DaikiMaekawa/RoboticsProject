#pragma once

#include <string>
#include <RDP/UserPose.h>
#include <vector>
#include <map>

namespace nui{

//typedef std::map<unsigned int, RDP::UserPose> PoseTable;

class Motion
{
    std::vector<RDP::UserPose> m_poses;
    unsigned int m_motionId;
    unsigned int m_detectTime;
    
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
    bool update(unsigned int elapsedTime);
    bool shouldDetectPose(RDP::UserPose &pose);
    unsigned int id(){ return m_motionId; }

};

} //namespace nui

