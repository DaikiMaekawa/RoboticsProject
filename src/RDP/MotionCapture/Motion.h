#pragma once

#include <string>
#include <RDP/UserJoint.h>
#include <vector>
#include <map>

typedef std::map<unsigned int, std::vector<RDP::UserJoint> > PoseTable;

class Motion
{
	PoseTable m_poses;
	unsigned int m_motionId;

public:
	static const int MAX_JOINT_NUM = 15;
	Motion();
	void saveAs(const std::string &file);
	void loadFrom(const std::string &file);
	void setPose(unsigned int poseId, std::vector<RDP::UserJoint> &joints);
	const std::vector<RDP::UserJoint>& pose(unsigned int poseId);

};

