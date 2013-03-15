#include <iostream>
#include "NUI.h"

using namespace nui;
using namespace std;

int main(){
    
    NUI nui;
	nui.startProcessing();

	while( 'e' != cv::waitKey(3) ){
            cout << "ok" << endl;
            nui.waitUpdateAll();
            
            cout << nui.isValid() << endl;

            vector<UserStatus> users = nui.detectUsers();
            NIMat depth = nui.depthImage();
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

