#ifndef TERMINAL_H
#define TERMINAL_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

class SerialCommunication{
    class Impl;
    boost::shared_ptr<Impl> pImpl;

public:
    SerialCommunication();
    bool open(const std::string &port,const int baud);
    int read(char *buffers, unsigned int len);    
    int readWithoutLocking(char *buffers, unsigned int len);
    int write(const char *buffers, unsigned int len);

};

#endif // TERMINAL_H
