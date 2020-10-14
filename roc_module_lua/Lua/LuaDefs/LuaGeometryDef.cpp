#include "stdafx.h"

#include "Lua/LuaDefs/LuaGeometryDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaFunction.h"
#include "Core/Core.h"
#include "Core/TaskHandler.h"
#include "Utils/LuaUtils.h"

void LuaGeometryDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Geometry", Create);
    LuaUtils::AddClassMethod(f_vm, "getBoundSphereRadius", GetBoundSphereRadius);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaGeometryDef::Create(lua_State *f_vm)
{
    // element Geometry(bool path [, function callback ])
    std::string l_path;
    LuaFunction l_callback;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_path);
    l_argStream.ReadNextFunction(l_callback);
    if(!l_argStream.HasErrors() && !l_path.empty())
    {
        if(l_callback.IsValid())
        {
            void *l_task = Core::GetInstance()->GetTaskHandler()->CreateGeometryTask(l_path, l_callback);
            l_argStream.PushPointer(l_task);
        }
        else
        {
            ROC::IGeometry *l_geometry = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateIGeometry(l_path);
            l_geometry ? l_argStream.PushElement(l_geometry) : l_argStream.PushBoolean(false);
        }
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaGeometryDef::GetBoundSphereRadius(lua_State *f_vm)
{
    // float Geometry:getBoundSphereRadius()
    ROC::IGeometry *l_geometry;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_geometry);
    !l_argStream.HasErrors() ? l_argStream.PushNumber(l_geometry->GetBoundSphereRadius()) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
