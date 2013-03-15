#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "UserStatus.h"
#include <XnCppWrapper.h>
#include "ImageDevice.h"

namespace nui{

class UserDetector
{   
    ImageDevice &m_device;
    xn::UserGenerator m_userGenerator;
    xn::SkeletonCapability m_skeleton;
    xn::UserGenerator initUserGenerator(const xn::Context &context);
    xn::SkeletonCapability initSkeletonGenerator();

    static void XN_CALLBACK_TYPE NewUser(xn::UserGenerator &generator,XnUserID id,void *pCookie);
    static void XN_CALLBACK_TYPE LostUser(xn::UserGenerator &generator,XnUserID id,void *pCookie);
    static void XN_CALLBACK_TYPE StartCalibration(xn::SkeletonCapability &capability,XnUserID id,void *pCookie);
    static void XN_CALLBACK_TYPE EndCalibration(xn::SkeletonCapability &capability,XnUserID id,XnBool bSuccess, void *pCookie);
    void userJointPos(XnSkeletonJoint jointType,UserStatus &user);

public:
    UserDetector(ImageDevice &useDevice,const xn::Context &context);
    NIMat userDepthMetaData(const UserStatus &user);
    unsigned int detectUserNum();
    const std::vector<UserStatus>& detectUsers();
    void updateAllUserStatus();
    bool isValid(){ return m_userGenerator.IsValid(); }
};

};  //namespace nui

