
#pragma once
#include "SerialCommunication.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class SPPBoard
{

public:
    SPPBoard(const std::string &port,int baud);
    void motor(int id,int duty);
    void lcdPuts(const std::string &str);
    void lcdClear();
    void commandCancel();
    void servo(int angle);
    bool open(const std::string &port,int baud);
    std::vector<float> gyroStatus();
    void calibrationGyroStatus();

private:
    class Impl;
    boost::shared_ptr<Impl> pImpl;

};

