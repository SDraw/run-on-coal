#pragma once

namespace ROC
{

class IAsyncManager;
class IElementManager;
class IInheritanceManager;
class ILogManager;
class INetworkManager;
class IPhysicsManager;
class IRenderManager;
class ISfmlManager;
class ISoundManager;
class IVRManager;

class ICore
{
public:
    virtual IAsyncManager* GetAsyncManager() const = 0;
    virtual IElementManager* GetElementManager() const = 0;
    virtual IInheritanceManager* GetInheritManager() const = 0;
    virtual ILogManager* GetLogManager() const = 0;
    virtual INetworkManager* GetNetworkManager() const = 0;
    virtual IPhysicsManager* GetPhysicsManager() const = 0;
    virtual IRenderManager* GetRenderManager() const = 0;
    virtual ISfmlManager* GetSfmlManager() const = 0;
    virtual ISoundManager* GetSoundManager() const = 0;
    virtual IVRManager* GetVRManager() const = 0;
};

}
