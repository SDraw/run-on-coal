#pragma once

namespace ROC
{

class IElementManager;
class ILogManager;
class INetworkManager;

class ICore
{
public:
    virtual IElementManager* GetElementManager() const = 0;
    virtual ILogManager* GetLogManager() const = 0;
    virtual INetworkManager* GetNetworkManager() const = 0;
};

}
