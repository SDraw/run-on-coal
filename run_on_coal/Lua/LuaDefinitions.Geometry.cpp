#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Geometry.h"

namespace ROC
{
namespace Lua
{

int geometryCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Geometry *l_geometry = LuaManager::m_corePointer->GetElementManager()->CreateGeometry(l_path);
        l_geometry ? lua_pushlightuserdata(f_vm, l_geometry) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int geometryDestroy(lua_State *f_vm)
{
    Geometry *l_geometry;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_geometry), ElementType::GeometryElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyGeometry(l_geometry);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}