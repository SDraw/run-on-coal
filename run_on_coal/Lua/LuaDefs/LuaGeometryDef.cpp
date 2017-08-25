#include "stdafx.h"

#include "Lua/LuaDefs/LuaGeometryDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Geometry/Geometry.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

void ROC::LuaGeometryDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Geometry", Create);
    LuaUtils::AddClassMethod(f_vm, "getBoundSphereRadius", GetBoundSphereRadius);
    LuaUtils::AddClassMethod(f_vm, "isLoaded", IsLoaded);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaGeometryDef::Create(lua_State *f_vm)
{
    // element Geometry(bool path [, bool async = false])
    std::string l_path;
    bool l_async = false;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadNextBoolean(l_async);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Geometry *l_geometry = LuaManager::GetCore()->GetElementManager()->CreateGeometry(l_path,l_async);
        l_geometry ? argStream.PushElement(l_geometry) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaGeometryDef::IsLoaded(lua_State *f_vm)
{
    // bool Geometry:isLoaded()
    Geometry *l_geometry;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_geometry);
    !argStream.HasErrors() ? argStream.PushBoolean(l_geometry->IsLoaded()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaGeometryDef::GetBoundSphereRadius(lua_State *f_vm)
{
    // float Geometry:getBoundSphereRadius()
    Geometry *l_geometry;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_geometry);
    !argStream.HasErrors() ? argStream.PushNumber(l_geometry->GetBoundSphereRadius()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
