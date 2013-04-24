#include "UserRecognitionClient.h"

namespace nui{

    UserRecognitionClient::UserRecognitionClient(ros::NodeHandle &node) : 
        m_node(node),
        m_startPub(node.advertise<std_msgs::Empty>(START_USER_RECOGNITION, 100))
    {
        sleep(1);
    }


}; //namespace nui

