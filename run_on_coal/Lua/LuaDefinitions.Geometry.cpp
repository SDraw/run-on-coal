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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Geometry *l_geometry = LuaManager::GetCore()->GetElementManager()->CreateGeometry(l_path);
        l_geometry ? argStream.PushPointer(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int geometryDestroy(lua_State *f_vm)
{
    Geometry *l_geometry;
    ArgReader argStream(f_vm);
    argStream.ReadElement(reinterpret_cast<void**>(&l_geometry), ElementType::GeometryElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetElementManager()->DestroyGeometry(l_geometry);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
