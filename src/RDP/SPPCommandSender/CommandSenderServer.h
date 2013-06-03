/*******************************************************************
 * Copyright (c) 2012 Daiki Maekawa
 *
 * @file CommandSenderServer.h
 * @brief server for command sender
 * @author Daiki Maekawa
 * @date 2012-12-01
 *******************************************************************/

#pragma once

#include <std_msgs/String.h>
#include "CommandSenderClient.h"
#include "SerialCommunication.h"

namespace spp{

    class CommandSenderServer{
        SerialCommunication m_port;
        ros::NodeHandle &m_node;
        ros::Subscriber m_cmdSub;
        void commandReceiveCb(const std_msgs::StringConstPtr &msg);

    public:
        CommandSenderServer(ros::NodeHandle &node);
        void runServer();
    
    };

} //namespace spp

