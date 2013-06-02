/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file SPPCommandSenderClient.h
 * @brief SPPCommand Sender Client
 * @author Daiki Maekawa
 * @date 2012-11-21
 *******************************************************************/

#pragma once
#include <ros/ros.h>

namespace spp{

    class SPPCommandSenderClient{
        ros::NodeHandle &m_node;
        ros::Publisher  m_lcdPutsPub;
    
    };

}

