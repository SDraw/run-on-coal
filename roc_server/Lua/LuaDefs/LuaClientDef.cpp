#include "stdafx.h"

#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Elements/Client.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

void ROC::LuaClientDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Client", nullptr);
    LuaUtils::AddClassMethod(f_vm, "disconnect", Disconnect);
    LuaUtils::AddClassMethod(f_vm, "sendData", SendData);
    LuaUtils::AddClassMethod(f_vm, "getID", GetID);
    LuaUtils::AddClassMethod(f_vm, "getAddress", GetAddress);
    LuaUtils::AddClassMethod(f_vm, "getPing", GetPing);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaClientDef::Disconnect(lua_State *f_vm)
{
    // bool Client:disconnect()
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetNetworkManager()->Disconnect(l_client);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::SendData(lua_State *f_vm)
{
    // bool Client:sendData(str data)
    Client *l_client;
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        bool l_result = Core::GetCore()->GetNetworkManager()->SendData(l_client, l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::GetID(lua_State *f_vm)
{
    // int Client:getID()
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    !argStream.HasErrors() ? argStream.PushInteger(l_client->GetID()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::GetAddress(lua_State *f_vm)
{
    // str int Client:getAddress()
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        std::string l_ip;
        unsigned short l_port;
        l_client->GetAddress(l_ip, l_port);
        argStream.PushText(l_ip);
        argStream.PushNumber(l_port);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::GetPing(lua_State *f_vm)
{
    // int Client:getPing()
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        int l_ping = Core::GetCore()->GetNetworkManager()->GetPing(l_client);
        argStream.PushNumber(l_ping);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
