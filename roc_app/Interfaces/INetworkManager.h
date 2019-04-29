#pragma once

namespace ROC
{

class INetworkManager
{
public:
    virtual bool Connect(const std::string &f_ip, unsigned short f_port) = 0;
    virtual bool Disconnect() = 0;
    virtual bool SendData(const std::string &f_data) = 0;

    virtual unsigned char GetNetworkState() const = 0;
    virtual int GetPing() const = 0;
};

}
