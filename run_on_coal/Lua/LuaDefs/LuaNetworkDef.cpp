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
    lua_register(f_vm, "networkConnect", Connect);
    lua_register(f_vm, "networkDisconnect", Disconnect);
    lua_register(f_vm, "networkSendData", SendData);
    lua_register(f_vm, "networkGetState", GetState);
    lua_register(f_vm, "networkGetPing", GetPing);
}

int ROC::LuaNetworkDef::Connect(lua_State *f_vm)
{
    // bool networkConnect(str ip, int port)
    std::string l_ip;
    unsigned short l_port;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_ip);
    argStream.ReadInteger(l_port);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetNetworkManager()->Connect(l_ip, l_port);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::Disconnect(lua_State *f_vm)
{
    // bool networkDisconnect
    ArgReader argStream(f_vm);
    bool l_result = Core::GetCore()->GetNetworkManager()->Disconnect();
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::SendData(lua_State *f_vm)
{
    // bool networkSendData(str data)
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetNetworkManager()->SendData(l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::GetState(lua_State *f_vm)
{
    // str networkGetState()
    ArgReader argStream(f_vm);
    argStream.PushText(g_NetworkStatesTable[Core::GetCore()->GetNetworkManager()->GetNetworkState()]);
    return argStream.GetReturnValue();
}
int ROC::LuaNetworkDef::GetPing(lua_State *f_vm)
{
    // int networkGetPing()
    ArgReader argStream(f_vm);
    argStream.PushInteger(Core::GetCore()->GetNetworkManager()->GetPing());
    return argStream.GetReturnValue();
}
