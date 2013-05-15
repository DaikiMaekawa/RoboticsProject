#include "UserRecognitionClient.h"

namespace nui{

    UserRecognitionClient::UserRecognitionClient(ros::NodeHandle &node) : 
        m_node(node)
    {
        sleep(1);
    }


}; //namespace nui

