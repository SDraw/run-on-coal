#include "stdafx.h"

#include "Lua/LuaDefs/LuaClientDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/NetworkManager.h"
#include "Elements/Client.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

void ROC::LuaClientDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Client", ClientCreateDummy);
    Utils::Lua::lua_registerClassMethod(f_vm, "disconnect", ClientDisconnect);
    Utils::Lua::lua_registerClassMethod(f_vm, "sendData", ClientSendData);
    Utils::Lua::lua_registerClassMethod(f_vm, "getID", ClientGetID);
    Utils::Lua::lua_registerClassMethod(f_vm, "getAddress", ClientGetAddress);
    Utils::Lua::lua_registerClassMethod(f_vm, "getPing", ClientGetPing);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaClientDef::ClientCreateDummy(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int ROC::LuaClientDef::ClientDisconnect(lua_State *f_vm)
{
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetNetworkManager()->Disconnect(l_client);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::ClientSendData(lua_State *f_vm)
{
    Client *l_client;
    std::string l_data;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        bool l_result = LuaManager::GetCore()->GetNetworkManager()->SendData(l_client, l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::ClientGetID(lua_State *f_vm)
{
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    !argStream.HasErrors() ? argStream.PushInteger(l_client->GetID()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaClientDef::ClientGetAddress(lua_State *f_vm)
{
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
int ROC::LuaClientDef::ClientGetPing(lua_State *f_vm)
{
    Client *l_client;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_client);
    if(!argStream.HasErrors())
    {
        int l_ping = LuaManager::GetCore()->GetNetworkManager()->GetPing(l_client);
        argStream.PushNumber(l_ping);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
