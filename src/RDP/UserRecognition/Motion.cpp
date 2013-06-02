#include "Motion.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

namespace nui{

Motion::Motion() : 
	m_motionId(0),
    m_detectTime(0),
    m_detectingPoseId(1),
    m_allPosesIsDetected(false)
{

}

bool Motion::saveAs(const string &file){
	using boost::lexical_cast;
	ofstream ofs(file.c_str());
	if(ofs){
		ofs << m_poses.size() << endl;

		for(int id=1; id <= m_poses.size() ;id++){
			
			if(!poseIsExist(id)){
				cout << "m_poses[" << id << "] is not found" << endl;
			}else{
				//const RDP::UserPose &pose = m_poses[id];
				const RDP::UserPose &uPose = pose(id);
                assert(Motion::MAX_JOINT_NUM == uPose.joints.size());
				ofs << uPose.waitTime << ",";
				for(int jointNo=0; jointNo < Motion::MAX_JOINT_NUM; jointNo++){
					ofs << uPose.joints[jointNo].pos.x << ","
						<< uPose.joints[jointNo].pos.y << ","
						<< uPose.joints[jointNo].pos.z << ","
						<< static_cast<int>(uPose.joints[jointNo].xIsKey) << ","
						<< static_cast<int>(uPose.joints[jointNo].yIsKey) << ","
						<< static_cast<int>(uPose.joints[jointNo].zIsKey) << ",";
				}
				ofs << "0x0" << endl;
			}
		}
		return true;
	}else{
		return false;
	}

}

bool Motion::loadFrom(const std::string &file){
	using boost::lexical_cast;
	std::ifstream ifs(file.c_str());

	if(ifs){
        std::list<std::string> ret;
        boost::split(ret, file, boost::is_any_of("/"));

		sscanf(ret.back().substr(0, 3).c_str(), "%d", &m_motionId); //TODO: erase '/'
        std::cout << "No." << m_motionId << " " << file << std::endl;

		m_poses.clear();
		
		char str[256];
		char linebuf[1024];
		
		ifs.getline(linebuf, 1024);
		std::string linestr(linebuf);
		const unsigned int maxPoseNum = lexical_cast<int>(linestr);

		for(int poseId=1; poseId <= maxPoseNum; poseId++){
			ifs.getline(linebuf, 1024);
			std::string linestr(linebuf);
			std::istringstream sstrm(linestr);
            
            RDP::UserPose uPose;
            uPose.joints.resize(Motion::MAX_JOINT_NUM);
            uPose.id = poseId;

			sstrm.getline(str, 10, ',');
            uPose.waitTime = lexical_cast<int>(str);

			for(int jointNo=0; jointNo < Motion::MAX_JOINT_NUM; jointNo++){
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].pos.x = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].pos.y = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].pos.z = lexical_cast<float>(str);
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].xIsKey = lexical_cast<int>(str);
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].yIsKey = lexical_cast<int>(str);
				sstrm.getline(str, 10, ',');
				uPose.joints[jointNo].zIsKey = lexical_cast<int>(str);
			}

            m_poses.push_back(uPose);
		}
		return true;
	}else{
        cout << "Motion::loadFrom: failed" << endl;
		return false;
	}
}

bool Motion::setPose(const RDP::UserPose &pose){
	
    for(int i=0; i < m_poses.size(); i++){
        if(pose.id == m_poses[i].id){
            m_poses[i] = pose;
            return true;
        }
    }

    std::cout << "m_poses.id" << pose.id << " is not found" << std::endl;
    return false;
    
}

const RDP::UserPose& Motion::pose(unsigned int poseId){

	for(int i=0; i < m_poses.size(); i++){
        if(poseId == m_poses[i].id){
            return m_poses[i];
        }
    }
    
    std::cout << "m_poses.id" << poseId << " is not found" << std::endl;
    assert(false);
    /* 
    if(poseIsExist(poseId)){
        pose = m_poses[poseId];
		ret = true;
	}else{
		std::cout << "m_poses[" << poseId << "] is not found" << std::endl;
		ret = false;
	}

	return ret;
    */
}

bool Motion::addPose(const RDP::UserPose &pose){
	
    if(poseIsExist(pose.id)){
        std::cout << "Add m_poses.id" << pose.id << "failed" << std::endl;
        return false;
    }

    m_poses.push_back(pose);
    return true;
    
}

bool Motion::erasePose(unsigned int poseId){
	//TODO implementation
	
    return false;
}

void Motion::update(float elapsedTime, bool isDetectedPose){
    //TODO implementation
    m_detectTime += elapsedTime;
    assert(poseIsExist(m_detectingPoseId));
    
    if(m_detectTime > pose(m_detectingPoseId).waitTime){
        cout << "timeout" << endl;
        m_detectingPoseId = 1;
        m_detectTime = 0;
        assert(poseIsExist(m_detectingPoseId));
        return;
    }
    
    if(isDetectedPose){
        if(poseIsExist(m_detectingPoseId+1)){
            cout << "shift next pose" << endl;
            m_detectingPoseId = ++m_detectingPoseId;
        }else{
            cout << "return first pose" << endl;
            m_detectingPoseId = 1;
            m_allPosesIsDetected = true;
            assert(poseIsExist(m_detectingPoseId));
        }
    }

}

const RDP::UserPose& Motion::shouldDetectPose(){
    //TODO
    cout << "Motion::shouldDetectPose: m_detectingPoseId = " << m_detectingPoseId <<  endl;
    assert(poseIsExist(m_detectingPoseId));
    return pose(m_detectingPoseId);
}

bool Motion::poseIsExist(unsigned int poseId){
    
    for(int i=0; i < m_poses.size(); i++){
        if(m_poses[i].id == poseId) return true;
    }
    
    return false;
}

bool Motion::allPosesIsDetected(){
    if(m_allPosesIsDetected){
        m_allPosesIsDetected = false;
        return true;
    }

    return false;
}

} //namespace nui
