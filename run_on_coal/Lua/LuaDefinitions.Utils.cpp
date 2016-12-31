#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/LogManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Utils.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_elementTypeName
{
    "geometry", "model", "animation", "scene", "camera", "light",
    "rendertarget", "shader", "sound", "texture", "font", "file", "collision"
};

int disabledFunction(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int logPrint(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_text);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetLogManager()->Log(l_text);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int isElement(lua_State *f_vm)
{
    void *l_pointer = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadPointer(&l_pointer);
    if(!argStream.HasErrors()) lua_pushboolean(f_vm, LuaManager::m_corePointer->GetMemoryManager()->IsValidMemoryPointer(l_pointer));
    else lua_pushboolean(f_vm, 0);
    return 1;
}

int getElementType(lua_State *f_vm)
{
    void *l_pointer = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadPointer(&l_pointer);
    if(!argStream.HasErrors())
    {
        int l_type = LuaManager::m_corePointer->GetMemoryManager()->GetMemoryPointerType(l_pointer);
        lua_pushstring(f_vm, (l_type != -1) ? g_elementTypeName[l_type].c_str() : "invalid");
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int getTickCount(lua_State *f_vm)
{
    lua_Integer l_tick = 0;
#ifdef _WIN32
    l_tick = static_cast<lua_Integer>(GetTickCount());
#elif __linux__
    struct timeval tv;
    if(!gettimeofday(&tv, NULL)) l_tick = static_cast<lua_Integer>((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
    lua_pushinteger(f_vm, l_tick);
    return 1;
}

}
}