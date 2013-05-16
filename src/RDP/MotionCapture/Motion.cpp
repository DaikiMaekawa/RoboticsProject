#include "Motion.h"
#include <iostream>
#include <boost/tuple/tuple.hpp>

unsigned int Motion::maxUserId(){
	unsigned int max = 0;
	for(int i=0; i < m_poses.size(); i++){
		if(max < m_poses[i].first) max = m_poses[i].first;
	}

	return max;
}

void Motion::saveAs(const std::string &file){

}

void Motion::loadFrom(const std::string &file){

}

void Motion::setUserStatus(unsigned int poseId, RDP::UserStatus &user){
	
	if(maxUserId()+1 == poseId){
		m_poses.push_back(Pose(poseId, user));
		return;
	}

	for(int i=0; i < m_poses.size(); i++){
		unsigned int id;
		RDP::UserStatus pose;
		boost::tie(id, pose) = m_poses[i];
		if(id == poseId){
			m_poses[i].second = user;
			return;
		}
	}

	std::cout << "Motion::setUserStatus out of range" << std::endl;
}

