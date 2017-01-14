#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Managers/LogManager.h"
#include "Elements/Client.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Network.h"

namespace ROC
{
namespace Lua
{

int networkDisconnectClient(lua_State *f_vm)
{
    Client *l_client;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_client), ElementType::ClientElement);
    if(!argStream.HasErrors())
    {

        bool l_result = LuaManager::m_corePointer->GetNetworkManager()->Disconnect(l_client);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int networkSendDataToClient(lua_State *f_vm)
{
    Client *l_client;
    std::string l_data;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_client), ElementType::ClientElement);
    argStream.ReadText(l_data);
    if(!argStream.HasErrors() && !l_data.empty())
    {
        bool l_result = LuaManager::m_corePointer->GetNetworkManager()->SendData(l_client, l_data);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int networkGetClientID(lua_State *f_vm)
{
    Client *l_client;
    std::string l_data;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_client), ElementType::ClientElement);
    !argStream.HasErrors() ? argStream.PushInteger(l_client->GetID()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
