#include "Motion.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace nui{

Motion::Motion() : 
	m_motionId(0)
{
}

void Motion::saveAs(const string &file){
	using boost::lexical_cast;
	ofstream ofs(file.c_str());
	if(ofs){
		ofs << m_poses.size() << endl;

		for(int id=0; id < m_poses.size() ;id++){
			
			if(m_poses.find(id) == m_poses.end()){
				cout << "m_poses[" << id << "] is not found" << endl;
			}else{
				const RDP::UserPose &pose = m_poses[id];
				assert(Motion::MAX_JOINT_NUM == pose.joints.size());
				ofs << pose.waitTime << ",";
				for(int jointNo=0; jointNo < Motion::MAX_JOINT_NUM; jointNo++){
					ofs << pose.joints[jointNo].pos.x << ","
						<< pose.joints[jointNo].pos.y << ","
						<< pose.joints[jointNo].pos.z << ","
						<< static_cast<int>(pose.joints[jointNo].xIsKey) << ","
						<< static_cast<int>(pose.joints[jointNo].yIsKey) << ","
						<< static_cast<int>(pose.joints[jointNo].zIsKey) << ",";
				}
				ofs << "0x0" << endl;
			}
		}
	}else{
		assert(false);
	}
}

void Motion::loadFrom(const std::string &file){
	using boost::lexical_cast;
	std::ifstream ifs(file.c_str());

	if(ifs){
		sscanf(file.substr(0, 3).c_str(), "%d", &m_motionId);
		std::cout << "No." << m_motionId << " " << file << std::endl;

		//m_poses.clear();
		m_poses.clear();
		
		char str[256];
		char linebuf[1024];
		
		ifs.getline(linebuf, 1024);
		std::string linestr(linebuf);
		const unsigned int maxPoseNum = lexical_cast<int>(linestr);

		for(int poseId=0; poseId < maxPoseNum; poseId++){
			ifs.getline(linebuf, 1024);
			std::string linestr(linebuf);
			std::istringstream sstrm(linestr);
			//m_poses[poseId] = vector<RDP::UserJoint>(Motion::MAX_JOINT_NUM);
			m_poses[poseId] = RDP::UserPose();
			m_poses[poseId].joints.resize(Motion::MAX_JOINT_NUM);
			
			sstrm.getline(str, 10, ',');
			m_poses[poseId].waitTime = lexical_cast<int>(str);

			for(int jointNo=0; jointNo < Motion::MAX_JOINT_NUM; jointNo++){
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].pos.x = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].pos.y = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].pos.z = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].xIsKey = lexical_cast<int>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].yIsKey = lexical_cast<int>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId].joints[jointNo].zIsKey = lexical_cast<int>(str);
			}
		}
	}else{
		assert(false);
	}
}

void Motion::setPose(unsigned int poseId, RDP::UserPose &pose){
	m_poses[poseId] = pose;
}

const RDP::UserPose& Motion::pose(unsigned int poseId){
	if(m_poses.find(poseId) == m_poses.end()){
		std::cout << "m_poses[" << poseId << "] is not found" << std::endl;
		RDP::UserPose pose;
		pose.joints.resize(Motion::MAX_JOINT_NUM);
		m_poses[poseId] = pose;
		//m_poses[poseId] = vector<RDP::UserJoint>(Motion::MAX_JOINT_NUM);
	}	
	
	return m_poses[poseId];
}

} //namespace nui

