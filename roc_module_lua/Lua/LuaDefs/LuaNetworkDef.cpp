#include "stdafx.h"

#include "Lua/LuaDefs/LuaNetworkDef.h"

#include "Lua/ArgReader.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

const std::string g_NetworkStatesTable[]
{
    "disconnected", "connecting", "connected", "disconnecting"
};

void LuaNetworkDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "networkConnect", Connect);
    lua_register(f_vm, "networkDisconnect", Disconnect);
    lua_register(f_vm, "networkSendData", SendData);
    lua_register(f_vm, "networkGetState", GetState);
    lua_register(f_vm, "networkGetPing", GetPing);
}

int LuaNetworkDef::Connect(lua_State *f_vm)
{
    // bool networkConnect(str ip, int port)
    std::string l_ip;
    unsigned short l_port = 42000;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_ip);
    argStream.ReadNextInteger(l_port);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->Connect(l_ip, l_port);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaNetworkDef::Disconnect(lua_State *f_vm)
{
    // bool networkDisconnect
    ArgReader argStream(f_vm);
    bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->Disconnect();
    argStream.PushBoolean(l_result);
    return argStream.GetReturnValue();
}

int LuaNetworkDef::SendData(lua_State *f_vm)
{
    // bool networkSendData(str data)
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->SendData(l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaNetworkDef::GetState(lua_State *f_vm)
{
    // str networkGetState()
    ArgReader argStream(f_vm);
    argStream.PushText(g_NetworkStatesTable[LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->GetNetworkState()]);
    return argStream.GetReturnValue();
}

int LuaNetworkDef::GetPing(lua_State *f_vm)
{
    // int networkGetPing()
    ArgReader argStream(f_vm);
    argStream.PushInteger(LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->GetPing());
    return argStream.GetReturnValue();
}
