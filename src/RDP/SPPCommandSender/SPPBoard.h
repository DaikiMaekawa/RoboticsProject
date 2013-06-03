
#pragma once
//#include "SerialCommunication.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class SPPBoard
{

public:
    //SPPBoard(const std::string &port,int baud);
    SPPBoard();
    std::string motor(int id,int duty);
    std::string lcdPuts(const std::string &str);
    std::string lcdClear();
    std::string commandCancel();
    std::string servo(int angle);
    //bool open(const std::string &port,int baud);
    //std::vector<float> gyroStatus();
    std::string calibrationGyroStatus();

private:
    class Impl;
    boost::shared_ptr<Impl> pImpl;

};

