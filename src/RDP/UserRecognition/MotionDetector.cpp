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
        
        if((int)fileVec.size() == 0){
            cout << "motion file not found" << endl;
        }

        for(int i=0; i < (int)fileVec.size(); i++){
            path &p = fileVec[i];
            if(is_directory(p)){

            }else{
                string fleaf = p.filename().string();
                string ext = p.extension().string();

                if(ext == ".txt"){
                    Motion motion;
                    cout << "motion.loadFrom(" << p.string() << ")" << endl;
                    motion.loadFrom(p.string());
                    m_motions.push_back(motion);
                }
            }
        }
    }else{
        cout << "dirpath not found" << endl;
    }
}

bool MotionDetector::isDetectedPose(Motion &target ,const RDP::UserStatus &user){
    assert(user.isTracking);
    RDP::UserPose targetPose = target.shouldDetectPose();
    assert(user.pose.joints.size() == targetPose.joints.size());
    for(int jointNo=0; jointNo < user.pose.joints.size(); jointNo++){
        float targetPos = 0, pos = 0;
        if(targetPose.joints[jointNo].xIsKey){
            targetPos = targetPose.joints[jointNo].pos.x;
            cout << "target pos x = " << targetPos << endl;
            pos       = user.pose.joints[jointNo].pos.x;
            cout << "user   pos x = " << pos << endl;
        }else if(targetPose.joints[jointNo].yIsKey){
            targetPos = targetPose.joints[jointNo].pos.y;
            cout << "target pos y = " << targetPos << endl;
            pos       = user.pose.joints[jointNo].pos.y;
            cout << "user   pos y = " << pos << endl;
        }else if(targetPose.joints[jointNo].zIsKey){
            targetPos = targetPose.joints[jointNo].pos.z;
            cout << "target pos z = " << targetPos << endl;
            pos       = user.pose.joints[jointNo].pos.z;
            cout << "user   pos z = " << pos << endl;
        }else{
            continue;
        }
        if(targetPos - 30 > pos || pos > targetPos + 30){
            cout << "isDetectedPose: false" << endl;
            return false;
        }else{
            cout << "matching pos" << endl;
        }
    }

    cout << "isDetectedPose: true" << endl;
    return true;
}

void MotionDetector::updateUsers(const std::vector<RDP::UserStatus> &users, float elapsedTime){
    cout << "updateUsers: users.size() = " << users.size() << endl;
    
    for(int i=0; i < users.size(); i++){
        for(int m=0; m < m_motions.size(); m++){
            if(users[i].isTracking){
                bool ret = isDetectedPose(m_motions[m], users[i]);
                m_motions[m].update(elapsedTime, ret);
                
                if(m_motions[m].allPosesIsDetected()){
                    cout << "allPosesIsDetected: true" << endl;
                    m_detectCb(m_motions[m].id());
                }
            }
        }
    }
}

} //namespace nui

