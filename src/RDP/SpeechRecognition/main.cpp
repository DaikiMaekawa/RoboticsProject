#include "SpeechRecognitionServer.h"
#include <ros/ros.h>

//#include "SpeechRecognition.h"
//#include <string>

//#include <boost/regex.hpp>

//#include "RDP/AddTwoInts.h"

/*
bool add(RDP::AddTwoInts::Request  &req,
         RDP::AddTwoInts::Response &res )
{
  res.sum = req.a + req.b;
  ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
  ROS_INFO("sending back response: [%ld]", (long int)res.sum);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "SpeechRecognition");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("add_two_ints", add);
  ROS_INFO("Ready to add two ints.");
  ros::spin();

  return 0;
}

int main(int argc, char *argv[]){
    ros::init(argc, argv, "SpeechRecognition"); 
    
    SpeechRecognition recog(argc,argv);
    cout << "recog.isValid() = " << recog.isValid() << endl;
    cout << "startRecognition = " << recog.startRecognition(5) << endl;

    vector<string> results = recog.resultsString();
    for(int i=0; i < results.size(); i++){
        boost::regex regex("こんにちは");
        boost::smatch match;
        
        if(boost::regex_search(results[i], match, regex)){
            cout << "Recognized : " << match << endl;
        }
    }
    return 0;
}
*/

int main(int argc, char *argv[])
{
  
  ros::init(argc, argv, "SpeechRecognition");
  ros::NodeHandle node;
  nui::SpeechRecognitionServer server(argc,argv,node);
  server.runServer();

}

