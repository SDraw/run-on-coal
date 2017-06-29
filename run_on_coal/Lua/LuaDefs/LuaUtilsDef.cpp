#include "stdafx.h"

#include "Lua/LuaDefs/LuaUtilsDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/LogManager.h"
#include "Managers/SfmlManager.h"
#include "Elements/Element.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

void ROC::LuaUtilsDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "dofile", DisabledFunction);
    lua_register(f_vm, "loadfile", DisabledFunction);
    lua_register(f_vm, "logPrint", LogPrint);
    lua_register(f_vm, "getTickCount", GetTick);
    lua_register(f_vm, "getTime", GetTime);
}

int ROC::LuaUtilsDef::DisabledFunction(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int ROC::LuaUtilsDef::LogPrint(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetLogManager()->Log(l_text);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaUtilsDef::GetTick(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    lua_Integer l_tick = 0;
    l_tick = static_cast<lua_Integer>(GetTickCount());
    argStream.PushInteger(l_tick);
    return argStream.GetReturnValue();
}
int ROC::LuaUtilsDef::GetTime(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushNumber(LuaManager::GetCore()->GetSfmlManager()->GetTime());
    return argStream.GetReturnValue();
}
