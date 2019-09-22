#pragma once

namespace ROC
{

class IAsyncManager;
class IElementManager;
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
    virtual IAsyncManager* GetIAsyncManager() const = 0;
    virtual IElementManager* GetIElementManager() const = 0;
    virtual ILogManager* GetILogManager() const = 0;
    virtual INetworkManager* GetINetworkManager() const = 0;
    virtual IPhysicsManager* GetIPhysicsManager() const = 0;
    virtual IRenderManager* GetIRenderManager() const = 0;
    virtual ISfmlManager* GetISfmlManager() const = 0;
    virtual ISoundManager* GetISoundManager() const = 0;
    virtual IVRManager* GetIVRManager() const = 0;
};

}
