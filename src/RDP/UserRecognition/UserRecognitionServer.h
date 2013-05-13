/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file UserRecognitionServer.h
 * @brief User Recognition
 * @author Daiki Maekawa
 * @date 2012-01-10
 *******************************************************************/

#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include "UserStatus.h"
#include "ImageDevice.h"

namespace nui{

    class UserRecognitionServer{
        class Impl;
        boost::shared_ptr<Impl> pImpl;    
        void waitUpdateAll();
    public:
        UserRecognitionServer(ros::NodeHandle &node);
		void runServer();
        NIMat userDepthImage(const UserStatus &user);
        const std::vector<UserStatus>& detectUsers();
        bool isValid() const;
    };

} //namespace nui

