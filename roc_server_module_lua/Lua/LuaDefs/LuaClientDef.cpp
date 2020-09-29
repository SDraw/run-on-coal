#include "stdafx.h"

#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Lua/LuaArgReader.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

void LuaClientDef::Init(lua_State *f_vm)
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

int LuaClientDef::Disconnect(lua_State *f_vm)
{
    // bool Client:disconnect()
    ROC::IClient *l_client;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_client);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->Disconnect(l_client);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaClientDef::SendData(lua_State *f_vm)
{
    // bool Client:sendData(str data)
    ROC::IClient *l_client;
    std::string l_data;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_client);
    l_argStream.ReadText(l_data);
    if(!l_argStream.HasErrors() && !l_data.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->SendData(l_client, l_data);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaClientDef::GetID(lua_State *f_vm)
{
    // int Client:getID()
    ROC::IClient *l_client;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_client);
    !l_argStream.HasErrors() ? l_argStream.PushInteger(l_client->GetID()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaClientDef::GetAddress(lua_State *f_vm)
{
    // str int Client:getAddress()
    ROC::IClient *l_client;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_client);
    if(!l_argStream.HasErrors())
    {
        std::string l_ip;
        unsigned short l_port;
        l_client->GetAddress(l_ip, l_port);
        l_argStream.PushText(l_ip);
        l_argStream.PushNumber(l_port);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaClientDef::GetPing(lua_State *f_vm)
{
    // int Client:getPing()
    ROC::IClient *l_client;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_client);
    if(!l_argStream.HasErrors())
    {
        int l_ping = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->GetPing(l_client);
        l_argStream.PushNumber(l_ping);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
