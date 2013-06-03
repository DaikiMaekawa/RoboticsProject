#include "CommandSenderServer.h"
#include <ros/ros.h>

int main(int argc, char *argv[]){
    ros::init(argc, argv, "SPPCommandSender");
    ros::NodeHandle node;
    spp::CommandSenderServer server(node);
    server.runServer();

    return 0;
}

