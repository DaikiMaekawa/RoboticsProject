#include "SpeechRecognition.h"
#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace nui;
using namespace std;

int main(int argc, char *argv[]){
        
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
