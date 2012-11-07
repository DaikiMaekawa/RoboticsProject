#include "Unity.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace nui;

Unity::Unity(int argc, char *argv[]) : 
    m_sRecog(argc,argv),
    m_uRecog()
{
    m_uRecog.startProcessing();
}

void Unity::waitStatusUpdate(){
    m_uRecog.waitUpdateAll();
    std::cout << "update all status" << std::endl;
}

const std::vector<UserStatus>& Unity::detectUsers(){
    return m_uRecog.detectUsers();
}

std::vector<std::string> Unity::recogWords(int timeout){
    m_sRecog.startRecognition(timeout);
    return m_sRecog.resultsString();
}

bool Unity::isValid() const{
    return m_uRecog.isValid() && m_sRecog.isValid();
}

