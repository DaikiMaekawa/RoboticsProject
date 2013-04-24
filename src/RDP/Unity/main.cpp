#include "Unity.h"
#include <ros/ros.h>

#if 0

#include <ros/ros.h>
#include <std_msgs/Empty.h>


int main(int argc, char *argv[]){
    ros::init(argc, argv, "Unity");
    ros::NodeHandle node;
    ros::Publisher pub = node.advertise<std_msgs::Empty>("startSpeechRecognition", 100);
    ros::Rate loopRate(5);
    
    for(int i=0; i < 3; i++){
       loopRate.sleep(); 
    }

    //while(ros::ok()){
        std_msgs::Empty msg;
        pub.publish(msg);
        ROS_INFO("publish startSpeechRecognition");
        ros::spinOnce();
        loopRate.sleep();
    //}   
    return 0;
}

#else

int main(int argc, char *argv[]){
    Unity unity(argc, argv);
#ifdef SHARE_DIR
    ROS_INFO("defined share_dir = %s", SHARE_DIR);
#else
    ROS_INFO("not defined share_dir");
#endif
    //ROS_INFO("CATKIN_PACKAGE_SHARE = %s",SHARE_DIR);
    sleep(5);
    unity.startSpeechRecognition();
    unity.startUserRecognition();
    while(ros::ok()) unity.statusUpdate();
}

#endif



/*
#include "ros/ros.h"
#include "RDP/AddTwoInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "add_two_ints_client");
  if (argc != 3)
  {
    ROS_INFO("usage: add_two_ints_client X Y");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<RDP::AddTwoInts>("add_two_ints");
  RDP::AddTwoInts srv;
  srv.request.a = atoll(argv[1]);
  srv.request.b = atoll(argv[2]);
  if (client.call(srv))
  {
    ROS_INFO("Sum: %ld", (long int)srv.response.sum);
  }
  else
  {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }

  return 0;
}
*/
