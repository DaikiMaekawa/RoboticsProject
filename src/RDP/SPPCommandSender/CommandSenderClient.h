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
#include <std_msgs/String.h>
#include <string>
#include "SPPBoard.h"

namespace spp{
    
    static const char * const SPPCOMMAND  = "SPPCommand";

    class CommandSenderClient{
        ros::NodeHandle &m_node;
        ros::Publisher  m_cmdPub;
        SPPBoard        m_cmd;
         
    public:
        CommandSenderClient(ros::NodeHandle &node);
        void lcdPuts(const std::string &str);
        void lcdClear();
        void motor(int rDuty, int lDuty);
        
    };

} //namespace spp

