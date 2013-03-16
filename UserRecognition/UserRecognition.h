/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file NUI.h
 * @brief NUI interface class
 * @author Daiki Maekawa
 * @date 2012-01-10
 *******************************************************************/

#pragma once
#include <boost/shared_ptr.hpp>
#include <vector>
#include "UserStatus.h"
#include "ImageDevice.h"
#include "Atlas/Singleton.h"

namespace nui{

    class UserRecognition{
        class Impl;
        boost::shared_ptr<Impl> pImpl;

    public:
        UserRecognition();
        void startProcessing();
        void waitUpdateAll();
        NIMat depthImage();
        NIMat rgbImage();
        NIMat userDepthImage(const UserStatus &user);
        unsigned int detectUserNum();
        const std::vector<UserStatus>& detectUsers();
        bool isValid() const;
    };

} //namespace nui

