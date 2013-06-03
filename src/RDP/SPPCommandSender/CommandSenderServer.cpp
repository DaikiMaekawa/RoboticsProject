#include "CommandSenderServer.h"
#include <string>
#include <iostream>

namespace spp{
    
    CommandSenderServer::CommandSenderServer(ros::NodeHandle &node) : 
        m_node(node),
        m_cmdSub(node.subscribe(SPPCOMMAND, 100, &CommandSenderServer::commandReceiveCb, this))
    {
        if(!m_port.open("/dev/ttyUSB0", 115200)){
            ROS_INFO("/dev/ttyUSB0 open failed");
        }
        sleep(1);
    }
    
    void CommandSenderServer::commandReceiveCb(const std_msgs::StringConstPtr &msg){
        ROS_INFO("receive SPPCommand");
        std::cout << "cmd = " << msg->data << std::endl;
        m_port.write(msg->data.c_str(), msg->data.length());
    }

    void CommandSenderServer::runServer(){
        ros::Rate loopRate(5);
        while(ros::ok()){
            ros::spinOnce();
            loopRate.sleep();
        }
    }

} //namespace spp
