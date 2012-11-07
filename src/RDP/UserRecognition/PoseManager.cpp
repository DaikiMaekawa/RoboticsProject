#include "PoseManager.h"

namespace nui{

PoseManager::PoseManager(const Motion &motion) :
    m_motion(motion),
    m_detectTime(0),
    m_poseId(1)
{
    //m_isValid = m_motion.pose(m_poseId, m_detectingPose);
}

void PoseManager::setShouldDetectPose(){
    /*
    if(!m_motion.pose(++m_poseId, m_detectingPose)){
        m_poseId = 1;
        m_isValid = m_motion.pose(m_poseId, m_detectingPose);
    }
    */
}

bool PoseManager::shouldDetectPose(unsigned int elapsedTime, RDP::UserPose &pose){
    /*
    if(!m_isValid) return false;

    m_detectTime += elapsedTime;
    if(m_detectTime > m_detectingPose.waitTime){
        setShouldDetectPose();
        if(m_isValid) pose = m_detectingPose;
        else          return false;
    }
    */
    return true;
}

} //namespace nui

