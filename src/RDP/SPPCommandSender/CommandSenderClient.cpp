#include "CommandSenderClient.h"
#include <std_msgs/String.h>

namespace spp{

    CommandSenderClient::CommandSenderClient(ros::NodeHandle &node) : 
        m_node(node),
        m_cmdPub(node.advertise<std_msgs::String>(SPPCOMMAND, 100))
    {
        sleep(1);
    }
    
    void CommandSenderClient::lcdPuts(const std::string &str){
        ROS_INFO("publish lcdPuts");
        std_msgs::String msg;
        msg.data = m_cmd.lcdPuts(str);
        m_cmdPub.publish(msg);
    }
    
    void CommandSenderClient::lcdClear(){
        ROS_INFO("publish lcdClear");
        std_msgs::String msg;
        msg.data = m_cmd.lcdClear();
        m_cmdPub.publish(msg);
    }
    
    void CommandSenderClient::motor(int rDuty, int lDuty){
        ROS_INFO("publish motor");
        std_msgs::String msg;
        msg.data = m_cmd.motor(rDuty, lDuty);
        m_cmdPub.publish(msg);
    }

} //namespace spp
