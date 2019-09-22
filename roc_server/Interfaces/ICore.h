#pragma once

namespace ROC
{

class IElementManager;
class ILogManager;
class INetworkManager;

class ICore
{
public:
    virtual IElementManager* GetIElementManager() const = 0;
    virtual ILogManager* GetILogManager() const = 0;
    virtual INetworkManager* GetINetworkManager() const = 0;
};

}
