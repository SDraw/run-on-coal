#include "stdafx.h"

#include "Lua/LuaDefs/LuaNetworkDef.h"

#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/LogManager.h"
#include "Lua/ArgReader.h"

namespace ROC
{

const std::string g_NetworkStatesTable[]
{ 
    "disconnected", "connecting", "connected", "disconnecting" 
};

}

void ROC::LuaNetworkDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "networkConnect", NetworkConnect);
    lua_register(f_vm, "networkDisconnect", NetworkDisconnect);
    lua_register(f_vm, "networkSendData", NetworkSendData);
    lua_register(f_vm, "networkGetState", NetworkGetState);
    lua_register(f_vm, "networkGetPing", NetworkGetPing);
}

int ROC::LuaNetworkDef::NetworkConnect(lua_State *f_vm)
{
    std::string l_ip;
    unsigned short l_port;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_ip);
    argStream.ReadInteger(l_port);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetNetworkManager()->Connect(l_ip, l_port);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::NetworkDisconnect(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    bool l_result = LuaManager::GetCore()->GetNetworkManager()->Disconnect();
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::NetworkSendData(lua_State *f_vm)
{
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_data);
    bool l_result = LuaManager::GetCore()->GetNetworkManager()->SendData(l_data);
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::NetworkGetState(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushText(g_NetworkStatesTable[LuaManager::GetCore()->GetNetworkManager()->GetNetworkState()]);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::NetworkGetPing(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushInteger(LuaManager::GetCore()->GetNetworkManager()->GetPing());
    return argStream.GetReturnValue();
}
