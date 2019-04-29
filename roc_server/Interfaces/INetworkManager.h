#pragma once

namespace ROC
{

class IClient;

class INetworkManager
{
public:
    virtual bool Disconnect(IClient *f_client) = 0;
    virtual bool SendData(IClient *f_client, const std::string &f_data) = 0;
    virtual int GetPing(IClient *f_client) const = 0;
};

}
