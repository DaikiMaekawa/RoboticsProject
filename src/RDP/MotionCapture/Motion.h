#pragma once

#include <string>
#include <RDP/DetectUsers.h>
#include <vector>
#include <map>

typedef std::pair<unsigned int, RDP::UserStatus> Pose;

class Motion
{
	std::vector<Pose> m_poses;
	unsigned int m_motionId;
	unsigned int maxPoseId();

public:
	Motion();
	void saveAs(const std::string &file);
	void loadFrom(const std::string &file);
	void setUserStatus(unsigned int poseId, RDP::UserStatus &user);

};

