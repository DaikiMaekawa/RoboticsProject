#include "Motion.h"
#include <iostream>
#include <fstream>
#include <boost/tuple/tuple.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

using namespace boost::filesystem;

Motion::Motion() : 
	m_motionId(0)
{
}

unsigned int Motion::maxPoseId(){
	unsigned int max = 0;
	for(int i=0; i < m_poses.size(); i++){
		if(max < m_poses[i].first) max = m_poses[i].first;
	}

	return max;
}

void Motion::saveAs(const std::string &file){
	
	std::ofstream ofs(file.c_str());
	if(!ofs){
		for(int i=0; i < m_poses.size() ;i++){
			unsigned int id;
			RDP::UserStatus pose;
			boost::tie(id, pose) = m_poses[i];
			for(int m=0; m < pose.joints.size() ;m++){
				ofs << pose.joints[m].pos.x << ","
				    << pose.joints[m].pos.y << ","
					<< pose.joints[m].pos.z << ",";
			}
			ofs << std::endl;
		}
	}
}

void Motion::loadFrom(const std::string &file){
	std::ifstream ifs(file.c_str());

	if(!ifs){
		char str[256];
		char linebuf[1024];

	}
	
	/*
	std::vector<path> files;
	path dir(file.c_str());
	directory_iterator end;
	
	if(exists(dir)){
		copy(directory_iterator(dir), directory_iterator(), back_inserter(files));

		for(int i=0; i < files.size(); i++){
			path &p = files[i];
			if(!is_directory(p)){
				std::string fleaf = p.filename().string();
				std::string ext = p.extension().string();
				
				sscanf(fleaf.substr(0, 3).c_str(), "%d", &m_motionId);
				ifstream ifs(p.)
			}
		}
	}
	*/
}

void Motion::setUserStatus(unsigned int poseId, RDP::UserStatus &user){
	
	if(maxPoseId()+1 == poseId){
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

