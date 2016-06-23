#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Cursor.h"

namespace ROC
{
namespace Lua
{

int cursorCreate(lua_State *f_vm)
{
    std::string l_path;
    bool l_sys = false;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_sys);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Cursor *l_cursor = LuaManager::m_corePointer->GetElementManager()->CreateCursor(l_path,l_sys);
    l_cursor ? lua_pushlightuserdata(f_vm,l_cursor) : lua_pushboolean(f_vm,0);
    return 1;
}
int cursorDestroy(lua_State *f_vm)
{
    Cursor *l_cursor;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_cursor,ElementType::CursorElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyCursor(l_cursor);
    lua_pushboolean(f_vm,result);
    return 1;
}

}
}
