#include "Unity.h"
#include <ros/ros.h>

int main(int argc, char *argv[]){
    Unity unity(argc, argv);

    sleep(3);
    while(ros::ok()) unity.statusUpdate();
}

