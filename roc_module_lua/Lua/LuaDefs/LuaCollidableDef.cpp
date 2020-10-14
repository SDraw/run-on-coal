#include "stdafx.h"

#include "Lua/LuaDefs/LuaCollidableDef.h"

#include "Lua/LuaArgReader.h"
#include "Core/Core.h"
#include "Utils/LuaUtils.h"

void LuaCollidableDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isCollidable", IsCollidable);
}

void LuaCollidableDef::AddHierarchyMethods(lua_State *f_vm)
{
    LuaUtils::AddClassMethod(f_vm, "setCollidableWith", SetCollidableWith);
}

int LuaCollidableDef::IsCollidable(lua_State *f_vm)
{
    // bool isCollidable(element obj)
    ROC::ICollidable *l_col = nullptr;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextElement(l_col);
    l_argStream.PushBoolean(l_col != nullptr);
    return l_argStream.GetReturnValue();
}

int LuaCollidableDef::SetCollidableWith(lua_State *f_vm)
{
    // bool Collidable:setCollidableWith(element collidable, bool state)
    ROC::ICollidable *l_colA;
    ROC::ICollidable *l_colB;
    bool l_state;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_colA);
    l_argStream.ReadElement(l_colB);
    l_argStream.ReadBoolean(l_state);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_colA->SetCollidableWith(l_colB, l_state);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
