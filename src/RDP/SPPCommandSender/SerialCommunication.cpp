#include "SerialCommunication.h"

using namespace std;

class SerialCommunication::Impl{

    boost::asio::io_service io_srv; 
    boost::asio::serial_port mport; 
    void asyncReadCallback(boost::asio::deadline_timer &timeout,std::size_t &out_numread, const boost::system::error_code& error, std::size_t bytes_transferred){
        timeout.cancel();
        if (!error) out_numread = bytes_transferred;
    }

public:
    Impl() : mport(io_srv){ }
    bool open(const std::string &portname,const int baud_rate){
        boost::system::error_code err;
        mport.open(portname, err);
        if (!err) {
            mport.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
            mport.set_option(boost::asio::serial_port_base::character_size(8));
            mport.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
            mport.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            mport.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        }
        return err ? false : true;
    }
    int read(char *buffers, unsigned int len){
        boost::system::error_code ec;   
        return mport.read_some(boost::asio::buffer(buffers, len), ec);
    }
    
    void timeoutCallback(boost::asio::serial_port& port, const boost::system::error_code& error){
        if (!error) {
            //timeout
            port.cancel();  
        }
    }
    int readWithoutLocking(char *buffers, unsigned int len ){
        size_t numread = 0;
        boost::asio::deadline_timer timeout(io_srv);
        while (numread < len) {
                size_t async_numread = 0;

                timeout.expires_from_now(boost::posix_time::milliseconds(300));
                timeout.async_wait(boost::bind(&SerialCommunication::Impl::timeoutCallback, this, boost::ref(mport), boost::asio::placeholders::error));
                
                boost::asio::async_read(mport,boost::asio::buffer(&buffers[numread], len-numread),
                                        boost::asio::transfer_at_least(len-numread), 
                                        boost::bind(&SerialCommunication::Impl::asyncReadCallback, this, boost::ref(timeout), boost::ref(async_numread), _1, _2));
                io_srv.run();
                if (async_numread == 0){
                    break;
                }
                numread += async_numread;
        }
        return numread;
    }
    
    int write(const char *buffers, unsigned int len){
        boost::system::error_code ec;
        return mport.write_some(boost::asio::buffer(buffers, len), ec);
    }
};

SerialCommunication::SerialCommunication() : pImpl(new Impl())
{

}

bool SerialCommunication::open(const string &port,const int baud){
    return pImpl->open(port,baud);
}

int SerialCommunication::read(char *buffers, unsigned int len){
    return pImpl->read(buffers,len);
}

int SerialCommunication::readWithoutLocking(char *buffers, unsigned int len){
    return pImpl->readWithoutLocking(buffers,len);
}

int SerialCommunication::write(const char *buffers, unsigned int len){
    return pImpl->write(buffers, len);
}

