#include "stdafx.h"

#include "Lua/LuaDefs/LuaGeometryDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/IGeometry.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaFunction.h"
#include "Module/LuaModule.h"
#include "Module/TaskHandler.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    argStream.ReadNextFunction(l_callback);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        if(l_callback.IsValid())
        {
            void *l_task = LuaModule::GetModule()->GetTaskHandler()->CreateGeometryTask(l_path, l_callback);
            argStream.PushPointer(l_task);
        }
        else
        {
            ROC::IGeometry *l_geometry = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIGeometry(l_path);
            l_geometry ? argStream.PushElement(l_geometry) : argStream.PushBoolean(false);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaGeometryDef::GetBoundSphereRadius(lua_State *f_vm)
{
    // float Geometry:getBoundSphereRadius()
    ROC::IGeometry *l_geometry;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_geometry);
    !argStream.HasErrors() ? argStream.PushNumber(l_geometry->GetBoundSphereRadius()) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
