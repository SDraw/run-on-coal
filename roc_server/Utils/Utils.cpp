#include "stdafx.h"

#include "Utils/Utils.h"

namespace ROC
{
namespace Utils
{

namespace Path
{

const std::regex g_UpRegex("(\\.\\.)+(\\/|\\\\)");
void EscapePath(std::string &f_path)
{
    std::string l_result;
    std::regex_replace(std::back_inserter(l_result), f_path.begin(), f_path.end(), g_UpRegex, "");
    f_path.assign(l_result);
}

}

namespace Enum
{

int ReadEnumVector(const std::vector<std::string> &f_vec, const std::string &f_val)
{
    int l_result = -1;
    for(auto iter = f_vec.begin(), iterEnd = f_vec.end(); iter != iterEnd; ++iter)
    {
        if(!iter->compare(f_val))
        {
            l_result = iter - f_vec.begin();
            break;
        }
    }
    return l_result;
}

}

namespace Lua
{

void lua_registerClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    lua_register(f_vm, f_name, f_func);
    luaL_newmetatable(f_vm, f_name);
    lua_pushvalue(f_vm, -1);
    lua_setfield(f_vm, -2, "__index");
    lua_pushboolean(f_vm, 0);
    lua_setfield(f_vm, -2, "__metatable");
}
void lua_registerClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func)
{
    lua_pushcfunction(f_vm, f_func);
    lua_setfield(f_vm, -2, f_name);
}

}

namespace Network
{

unsigned char GetPacketIdentifier(RakNet::Packet *f_packet)
{
    unsigned char l_result = 255U;
    if(f_packet)
    {
        if(f_packet->data[0] == ID_TIMESTAMP)
        {
            RakAssert(f_packet->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
            l_result = f_packet->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
        }
        else l_result = f_packet->data[0];
    }
    return l_result;
}

}

}
}
