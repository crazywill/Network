#ifndef IPROCESSCONNECTION_H
#define IPROCESSCONNECTION_H

#include <string>
class IProcessConnection
{
public:
    virtual void write(const std::string & str) = 0;
    virtual std::string read() = 0;
    virtual int localFD() = 0;
    virtual int remoteFD() = 0;

//private:
//    pid_t m_leftPid;
//    pid_t m_rightPid;
//    int m_leftFD;
//    int m_rightFD;

};

#endif // IPROCESSCONNECTION_H
