#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Geometry/Geometry.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Geometry.h"

namespace ROC
{
namespace Lua
{

int geometryCreate(lua_State *f_vm)
{
    std::string l_path;
    bool l_async = false;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_async);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Geometry *l_geometry = LuaManager::GetCore()->GetElementManager()->CreateGeometry(l_path,l_async);
        l_geometry ? argStream.PushPointer(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
