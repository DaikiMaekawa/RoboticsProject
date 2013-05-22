#pragma once

#include <string>
//#include <RDP/UserJoint.h>
#include <RDP/UserPose.h>
#include <vector>
#include <map>

namespace nui{

//typedef std::map<unsigned int, std::vector<RDP::UserJoint> > PoseTable;
typedef std::map<unsigned int, RDP::UserPose> PoseTable;

class Motion
{
	PoseTable m_poses;
	unsigned int m_motionId;

public:
	static const int MAX_JOINT_NUM = 15;
	Motion();
	void saveAs(const std::string &file);
	void loadFrom(const std::string &file);
	//void setPose(unsigned int poseId, std::vector<RDP::UserJoint> &joints);
	void setPose(unsigned int poseId, RDP::UserPose &pose);
	const RDP::UserPose& pose(unsigned int poseId);
	//const std::vector<RDP::UserJoint>& pose(unsigned int poseId);
	unsigned int id(){ return m_motionId; }
};

} //namespace nui

