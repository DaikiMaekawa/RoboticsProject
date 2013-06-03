#include "Unity.h"
#include <ros/ros.h>
#include <vector>
#include <iostream>

int main(int argc, char *argv[]){
    Unity unity(argc, argv);

    sleep(5);
    
    while(ros::ok()){
        unity.statusUpdate();
        std::vector<unsigned int> motions = unity.detectMotionId();
        for(int i=0; i < motions.size(); i++){
            switch(motions[i]){
                case 1: //hand up right
                    unity.motor(40, 20);
                break;
                case 2: //hand up left
                    unity.motor(20, 40);
                break;
            }
        }
    }
}

