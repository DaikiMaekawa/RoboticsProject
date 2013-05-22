#pragma once
#include <vector>
#include <XnCppWrapper.h>

namespace nui{

struct UserJoint{
    float x;
    float y;
    float z;
    XnSkeletonJoint jointType;

    UserJoint(XnSkeletonJoint jointType,float x=0, float y=0, float z=0) : 
        jointType(jointType), x(x), y(y), z(z)
    {

    }
};

class UserStatus{
    std::vector<UserJoint> joints;

public:
    int id;
    bool isTracking;
    static const int MAX_JOINT_NUM = 24;

    const UserJoint& joint(XnSkeletonJoint jointType){
        for(unsigned int i=0; i < joints.size(); i++)
            if(joints[i].jointType == jointType) return joints[i];
        
        assert(false);
    }

    void setJoint(const UserJoint &uJoint){
        for(unsigned int i=0; i < joints.size(); i++){
            if(joints[i].jointType == uJoint.jointType){
                joints[i] = uJoint;
                return;
            }
        }

        assert(false);
    }

    UserStatus(int id, bool isTracking=false) : 
        id(id), isTracking(isTracking)
    {
        for(int i=1; i <= MAX_JOINT_NUM; i++)
            joints.push_back( UserJoint((XnSkeletonJoint)i) );
    }
};

}; //namespace nui
