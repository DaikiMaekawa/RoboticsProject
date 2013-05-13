#include "SpeechRecognitionClient.h"

namespace nui{
    
    SpeechRecognitionClient::SpeechRecognitionClient(ros::NodeHandle &node) : 
        m_node(node),
		m_startPub(node.advertise<std_msgs::Empty>(START_SPEECH_RECOGNITION, 100)),
		m_pausePub(node.advertise<std_msgs::Empty>(PAUSE_SPEECH_RECOGNITION, 100)),
		m_finishPub(node.advertise<std_msgs::Empty>(FINISH_SPEECH_RECOGNITION, 100))
    {
        sleep(1);        
    }
    

}; //namespace nui
