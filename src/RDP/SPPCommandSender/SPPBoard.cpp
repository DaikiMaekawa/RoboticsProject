#include "SPPBoard.h"
#include "SPPCommand.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace std;

class SPPBoard::Impl
{
    //SerialCommunication m_com;
    //bool m_isConnect;

public:
    /*
    Impl(const string &port,int baud) : 
        m_isConnect(m_com.open(port,baud))
    {

    }
    */

    Impl() { }

    string motor(int id, int duty){
        return boost::lexical_cast<string>(SET_MOTOR_DUTY) + "," + boost::lexical_cast<string>(id) + "," + boost::lexical_cast<string>(duty) + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }

    string lcdPuts(const string &str){
        return boost::lexical_cast<string>(LCD_PUTS) + "," + str + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }

    string lcdClear(){
        return boost::lexical_cast<string>(LCD_CLEAR) + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }

    string commandCancel(){
        return boost::lexical_cast<string>(COMMAND_CANCEL) + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }

    string servo(int angle){
        return boost::lexical_cast<string>(SET_SERVO_ANGLE) + "," + boost::lexical_cast<string>(angle) + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }
    
    /*
    bool open(const string &port,int baud){
        if(!m_isConnect) m_isConnect = m_com.open(port,baud);
        return m_isConnect;
    }
    */
   
    /*
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
           
        //BOOST_FOREACH(string axis, gyro){
        //    ret.push_back(boost::lexical_cast<float>(axis));
        //}
        
        return ret;
    }
    */

    string calibrationGyroStatus(){
        return boost::lexical_cast<string>(CALIBRATION_GYRO) + "\r";
        //m_com.write(cmd.c_str(),cmd.length());
    }
};
/*
SPPBoard::SPPBoard(const string &port,int baud) : 
    pImpl(new Impl(port,baud))
{

}
*/

SPPBoard::SPPBoard() : pImpl(new Impl())
{

}

string SPPBoard::motor(int id,int duty){
    return pImpl->motor(id,duty);
}

string SPPBoard::lcdPuts(const string &str){
    return pImpl->lcdPuts(str);
}

string SPPBoard::lcdClear(){
    return pImpl->lcdClear();
}

string SPPBoard::commandCancel(){
    return pImpl->commandCancel();
}

string SPPBoard::servo(int angle){
    return pImpl->servo(angle);
}
/*
bool SPPBoard::open(const string &port,int baud){
    return pImpl->open(port,baud);
}
*/
/*
vector<float> SPPBoard::gyroStatus(){
    return pImpl->gyroStatus();
}
*/
string SPPBoard::calibrationGyroStatus(){
    return pImpl->calibrationGyroStatus();
}

