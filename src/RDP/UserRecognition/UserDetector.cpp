#include "UserDetector.h"
#include <boost/foreach.hpp>
#include "XnErrorChecker.h"
#include <iostream>

namespace nui{

static std::vector<UserStatus> g_users;

void XN_CALLBACK_TYPE UserDetector::NewUser(xn::UserGenerator &generator,XnUserID id,void* pCookie){
    std::cout << "NewUserId = " << id << std::endl; 
    generator.GetSkeletonCap().RequestCalibration(id,TRUE);
    g_users.push_back(UserStatus(id));
    
}

void XN_CALLBACK_TYPE UserDetector::LostUser(xn::UserGenerator &generator,XnUserID id,void* pCookie){
    std::cout << "LostUserId = " << id << std::endl; 
    for(unsigned int i=0; i < g_users.size(); i++)
        if(g_users[i].id == id) g_users.erase(g_users.begin() + i);
}

void XN_CALLBACK_TYPE UserDetector::StartCalibration(xn::SkeletonCapability& capability, XnUserID id, void* pCookie){
    std::cout << "StartCalibrationId = " << id << std::endl;
}

void XN_CALLBACK_TYPE UserDetector::EndCalibration(xn::SkeletonCapability& capability, XnUserID id, XnBool bSuccess, void* pCookie){
    std::cout << "EndCalibrationId = " << id << std::endl;
    bSuccess ? capability.StartTracking(id) : capability.RequestCalibration(id,TRUE);
}

UserDetector::UserDetector(ImageDevice& device,const xn::Context &context) : 
    m_device(device),
    m_userGenerator(initUserGenerator(context)),
    m_skeleton(initSkeletonGenerator())
{

}

xn::UserGenerator UserDetector::initUserGenerator(const xn::Context &context){
    g_users.clear();
    xn::UserGenerator userGenerator;
    xnErrorCheck(context.FindExistingNode(XN_NODE_TYPE_USER,userGenerator));
    XnCallbackHandle userCallbacks;
    xnErrorCheck(userGenerator.RegisterUserCallbacks(&UserDetector::NewUser,&UserDetector::LostUser,NULL,userCallbacks));
	
    return userGenerator;
}

xn::SkeletonCapability UserDetector::initSkeletonGenerator(){

    xn::SkeletonCapability skeleton = m_userGenerator.GetSkeletonCap();
    std::string pose;   //TODO not used
    xnErrorCheck(skeleton.GetCalibrationPose(&pose[0]));
    XnCallbackHandle calibCallbacks;
    xnErrorCheck(skeleton.RegisterCalibrationCallbacks(&UserDetector::StartCalibration,&UserDetector::EndCalibration,NULL,calibCallbacks));
    xnErrorCheck(skeleton.SetSkeletonProfile(XN_SKEL_PROFILE_ALL));
    //xnErrorCheck(skeleton.SetSkeletonProfile(XN_SKEL_PROFILE_UPPER));
    return skeleton;

}

NIMat UserDetector::userDepthMetaData(const UserStatus &user){
	xn::SceneMetaData smd;
	m_userGenerator.GetUserPixels(user.id,smd);
	//NIMat depth = m_device.depthImage();
    NIMat rgb = m_device.rgbImage();
    //assert(smd.YRes() == depth->rows() && smd.XRes() == depth->cols());
	
	unsigned int pos = 0;
	for(unsigned int y=0; y < smd.YRes(); y++){
        for(unsigned int x=0; x < smd.XRes(); x++){
            XnLabel label = smd(x,y);
        
            if(label == user.id){
                rgb->data[pos++] = 0;
                rgb->data[pos++] = 0;
                rgb->data[pos++] = 0;
            }else{
                rgb->data[pos++] = 255;
                rgb->data[pos++] = 255;
                rgb->data[pos++] = 255;
            }
        }
	}

    return rgb;
}

const std::vector<UserStatus>& UserDetector::detectUsers(){
    return g_users;
}

void UserDetector::updateAllUserStatus(){
    BOOST_FOREACH(UserStatus &user,g_users){
        user.isTracking = m_userGenerator.GetSkeletonCap().IsTracking(user.id);
        if(user.isTracking){
            for(int i=1; i <= UserStatus::MAX_JOINT_NUM; i++) userJointPos((XnSkeletonJoint)i, user);
        }
    }
}

void UserDetector::userJointPos(XnSkeletonJoint jointType,UserStatus &user){
    if(user.isTracking){
        XnSkeletonJointPosition pos;
        m_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user.id,jointType,pos);
        m_device.convertRealWorldToProjective(pos.position.X,pos.position.Y,pos.position.Z);
        user.setJoint(UserJoint(jointType, pos.position.X, pos.position.Y, pos.position.Z));
    }
}

}; //namespace nui

