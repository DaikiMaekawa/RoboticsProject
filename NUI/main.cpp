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

/*
int main(){
    xn::Context context;
    XnStatus nRetVal = context.Init();
    if (nRetVal != XN_STATUS_OK) {
        throw std::runtime_error(xnGetStatusString(nRetVal));
    }

    // デバイスを作成する(XMLからの生成だと、デバイスがないといわれる)
    xn::AudioGenerator audio;
    nRetVal = audio.Create(context);
    if (nRetVal != XN_STATUS_OK) {
        throw std::runtime_error(xnGetStatusString(nRetVal));
    }

    return 0;

}
*/
