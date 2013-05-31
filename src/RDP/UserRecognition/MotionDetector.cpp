#include "MotionDetector.h"
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <cassert>

using namespace std;
using namespace boost::filesystem;

namespace nui{

MotionDetector::MotionDetector()
{

}

void MotionDetector::loadFromMotionFiles(const string &dirpath){
    vector<path> fileVec;
    path dir(dirpath);
    directory_iterator end;

    if(exists(dir)){
        m_motions.clear();
        copy(directory_iterator(dir), directory_iterator(), back_inserter(fileVec));

        for(int i=0; i < (int)fileVec.size(); i++){
            path &p = fileVec[i];
            if(is_directory(p)){

            }else{
                string fleaf = p.filename().string();
                string ext = p.extension().string();

                if(ext == ".txt"){
                    Motion motion;
                    motion.loadFrom(p.filename().string());
                    m_motions.push_back(motion);
                }
            }
        }
    }
}

bool MotionDetector::isDetectedPose(Motion &target ,const RDP::UserStatus &user, unsigned int elapsedTime){
    assert(user.isTracking);
    RDP::UserPose targetPose = target.shouldDetectPose();
    assert(user.pose.joints.size() == targetPose.joints.size());
    for(int jointNo=0; jointNo < user.pose.joints.size(); jointNo++){
        float targetPos = 0, pos = 0;
        if(targetPose.joints[jointNo].xIsKey){
            targetPos = targetPose.joints[jointNo].pos.x;
            pos       = user.pose.joints[jointNo].pos.x;
        }else if(targetPose.joints[jointNo].yIsKey){
            targetPos = targetPose.joints[jointNo].pos.y;
            pos       = user.pose.joints[jointNo].pos.y;
        }else if(targetPose.joints[jointNo].zIsKey){
            targetPos = targetPose.joints[jointNo].pos.z;
            pos       = user.pose.joints[jointNo].pos.z;
        }else{
            continue;
        }
        if(targetPos - 100 > pos && pos > targetPos + 100){
            std::cout << "isDetectedPose: false" << std::endl;
            return false;
        }
    }

    std::cout << "isDetectedPose: true" << std::endl;
    return true;
}

void MotionDetector::updateUsers(const std::vector<RDP::UserStatus> &users, unsigned int elapsedTime){
    for(int i=0; i < users.size(); i++){
        for(int m=0; m < m_motions.size(); m++){
            bool ret = isDetectedPose(m_motions[m], users[i], elapsedTime);
            m_motions[m].update(elapsedTime, ret);
            
            if(m_motions[m].allPosesIsDetected()){
                std::cout << "allPosesIsDetected: true" << std::endl;
                m_detectCb(m_motions[m].id());
            }
        }
    }
}

} //namespace nui

