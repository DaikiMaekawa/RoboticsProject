#include "SPPBoard.h"
#include "SerialCommunication.h"
#include "SPPCommand.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace std;

class SPPBoard::Impl
{
    SerialCommunication m_com;
    bool m_isConnect;

public:
    Impl(const string &port,int baud) : 
        m_isConnect(m_com.open(port,baud))
    {
        
    }
    void motor(int id,int duty){
        string cmd = boost::lexical_cast<string>(SET_MOTOR_DUTY) + "," + boost::lexical_cast<string>(id) + "," + boost::lexical_cast<string>(duty) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }

    void lcdPuts(const string &str){
        string cmd = boost::lexical_cast<string>(LCD_PUTS) + "," + str + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }

    void lcdClear(){
        string cmd = boost::lexical_cast<string>(LCD_CLEAR) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }

    void commandCancel(){
        string cmd = boost::lexical_cast<string>(COMMAND_CANCEL) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }

    void servo(int angle){
        string cmd = boost::lexical_cast<string>(SET_SERVO_ANGLE) + "," + boost::lexical_cast<string>(angle) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }

    bool open(const string &port,int baud){
        if(!m_isConnect) m_isConnect = m_com.open(port,baud);
        return m_isConnect;
    }

    vector<float> gyroStatus(){
        string cmd = boost::lexical_cast<string>(GET_GYRO) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
        char buffer[256];
        m_com.readWithoutLocking(buffer,256);
        cout << buffer << endl;
        string buf = buffer;
        vector<string> gyro;
        boost::split(gyro,buf,boost::is_any_of(","));
        
        vector<float> ret;
        
        for(int i=0;i < gyro.size(); i++)
            cout << gyro[i] << endl;
            //ret.push_back(boost::lexical_cast<float>(gyro[i]));
           
        /*
        BOOST_FOREACH(string axis, gyro){
            ret.push_back(boost::lexical_cast<float>(axis));
        }
        */
        
        return ret;
    }

    void calibrationGyroStatus(){
        string cmd = boost::lexical_cast<string>(CALIBRATION_GYRO) + "\r";
        m_com.write(cmd.c_str(),cmd.length());
    }
};

SPPBoard::SPPBoard(const string &port,int baud) : 
    pImpl(new Impl(port,baud))
{

}

void SPPBoard::motor(int id,int duty){
    pImpl->motor(id,duty);
}

void SPPBoard::lcdPuts(const string &str){
    pImpl->lcdPuts(str);
}

void SPPBoard::lcdClear(){
    pImpl->lcdClear();
}

void SPPBoard::commandCancel(){
    pImpl->commandCancel();
}

void SPPBoard::servo(int angle){
    pImpl->servo(angle);
}

bool SPPBoard::open(const string &port,int baud){
    return pImpl->open(port,baud);
}

vector<float> SPPBoard::gyroStatus(){
    return pImpl->gyroStatus();
}

void SPPBoard::calibrationGyroStatus(){
    pImpl->calibrationGyroStatus();
}

