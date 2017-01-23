#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/LogManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Network.h"

namespace ROC
{
namespace Lua
{

const std::string g_networkStateTable[] = { "disconnected", "connecting", "connected", "disconnecting" };

int networkConnect(lua_State *f_vm)
{
    std::string l_ip;
    unsigned short l_port;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_ip);
    argStream.ReadInteger(l_port);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetNetworkManager()->Connect(l_ip, l_port);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int networkDisconnect(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    bool l_result = LuaManager::m_corePointer->GetNetworkManager()->Disconnect();
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}
int networkSendData(lua_State *f_vm)
{
    std::string l_data;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_data);
    bool l_result = LuaManager::m_corePointer->GetNetworkManager()->SendData(l_data);
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}
int networkGetState(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.PushText(g_networkStateTable[LuaManager::m_corePointer->GetNetworkManager()->GetNetworkState()]);
    return argStream.GetReturnValue();
}
int networkGetPing(lua_State *f_vm)
{
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.PushInteger(LuaManager::m_corePointer->GetNetworkManager()->GetPing());
    return argStream.GetReturnValue();
}

}
}
