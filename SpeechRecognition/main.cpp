#include "SpeechRecognition.h"
#include <iostream>
#include <string>

using namespace nui;
using namespace std;

int main(int argc, char *argv[]){
    SpeechRecognition recog(argc,argv);
    cout << "recog.isValid() = " << recog.isValid() << endl;
    
    cout << "startRecognition = " << recog.startRecognition(5) << endl;

    vector<string> results = recog.resultsString();
    for(int i=0; i < results.size(); i++){
        cout << "ret[" << i << "] = " << results[i] << endl;
    }
    
    return 0;
}
