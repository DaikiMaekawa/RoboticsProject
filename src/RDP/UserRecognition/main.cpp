#include <iostream>
#include "UserRecognitionServer.h"

int main(int argc, char *argv[]){
    ros::init(argc,argv,"UserRecognition");
    ros::NodeHandle node;
    nui::UserRecognitionServer server(node);
    server.runServer();

    return 0;
}

/*
using namespace nui;
using namespace std;

int main(){
    
    UserRecognition recog;
    recog.startProcessing();

    while( 'e' != cv::waitKey(3) ){
        cout << "ok" << endl;
        recog.waitUpdateAll();
        
        cout << recog.isValid() << endl;

        vector<UserStatus> users = recog.detectUsers();
        NIMat depth = recog.depthImage();
        for(int i=0; i < users.size(); i++){
            if(users[i].isTracking){
                UserJoint joint = users[i].joint(XN_SKEL_HEAD);
                cv::circle(*depth,cvPoint((int)joint.x,(int)joint.y), 5, cv::Scalar(255,255,255), 5);
            }
        }
        cv::imshow("depth",*depth);
    }
    
    return 0;
}

*/

