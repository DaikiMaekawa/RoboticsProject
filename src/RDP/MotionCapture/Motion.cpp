#include "Motion.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;

Motion::Motion() : 
	m_motionId(0)
{
}

void Motion::saveAs(const string &file){
	using boost::lexical_cast;
	ofstream ofs(file.c_str());
	if(ofs){
		for(int id=0; id < m_poses.size() ;id++){
			
			if(m_poses.find(id) == m_poses.end()){
				cout << "m_poses[" << id << "] is not found" << endl;
			}else{
				const vector<RDP::UserJoint> &pose = m_poses[id];
				assert(Motion::MAX_JOINT_NUM == pose.size());
				for(int m=0; m < pose.size() ;m++){
					ofs << pose[m].pos.x << ","
						<< pose[m].pos.y << ","
						<< pose[m].pos.z << ","
						<< static_cast<int>(pose[m].xIsKey) << ","
						<< static_cast<int>(pose[m].yIsKey) << ","
						<< static_cast<int>(pose[m].zIsKey) << ",";
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
		m_poses.clear();
		
		char str[256];
		char linebuf[1024];
		unsigned int poseId = 0;

		while(!ifs.eof()){
			ifs.getline(linebuf, 1024);
			std::string linestr(linebuf);
			std::istringstream sstrm(linestr);
			m_poses[poseId] = vector<RDP::UserJoint>(Motion::MAX_JOINT_NUM);
			
			for(int jointNo=0; jointNo < Motion::MAX_JOINT_NUM; jointNo++){
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].pos.x = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].pos.y = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].pos.z = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].xIsKey = lexical_cast<bool>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].yIsKey = lexical_cast<bool>(str);
				sstrm.getline(str, 10, ',');
				m_poses[poseId][jointNo].zIsKey = lexical_cast<bool>(str);

			}

			poseId++;
		}
	}else{
		assert(false);
	}
}

void Motion::setPose(unsigned int poseId, vector<RDP::UserJoint> &joints){
	m_poses[poseId] = joints;
}

const vector<RDP::UserJoint>& Motion::pose(unsigned int poseId){
	if(m_poses.find(poseId) == m_poses.end()){
		std::cout << "m_poses[" << poseId << "] is not found" << std::endl;
		m_poses[poseId] = vector<RDP::UserJoint>(Motion::MAX_JOINT_NUM);
	}	
	
	return m_poses[poseId];
}

