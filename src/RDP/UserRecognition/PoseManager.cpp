#include "PoseManager.h"

namespace nui{

PoseManager::PoseManager(const Motion &motion) : 
    m_motion(motion),
    m_detectTime(0)
{
}

const RDP::UserPose& PoseManager::shouldDetectPose(unsigned int elapsedTime){
    m_detectTime += elapsedTime;

}

} //namespace nui
