#include "stdafx.h"

#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/INetworkManager.h"
#include "Interfaces/IClient.h"
#include "Lua/ArgReader.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->Disconnect(l_client);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaClientDef::SendData(lua_State *f_vm)
{
    // bool Client:sendData(str data)
    ROC::IClient *l_client;
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->SendData(l_client, l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaClientDef::GetID(lua_State *f_vm)
{
    // int Client:getID()
    ROC::IClient *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    !argStream.HasErrors() ? argStream.PushInteger(l_client->GetID()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int LuaClientDef::GetAddress(lua_State *f_vm)
{
    // str int Client:getAddress()
    ROC::IClient *l_client;
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
int LuaClientDef::GetPing(lua_State *f_vm)
{
    // int Client:getPing()
    ROC::IClient *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        int l_ping = LuaModule::GetModule()->GetEngineCore()->GetINetworkManager()->GetPing(l_client);
        argStream.PushNumber(l_ping);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
