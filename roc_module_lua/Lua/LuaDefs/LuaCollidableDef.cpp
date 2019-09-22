#include "stdafx.h"

#include "Lua/LuaDefs/LuaCollidableDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/ICollidable.h"
#include "Lua/ArgReader.h"
#include "Module/LuaModule.h"
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
    ROC::ICollidable *l_col = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_col);
    argStream.PushBoolean(l_col != nullptr);
    return argStream.GetReturnValue();
}

int LuaCollidableDef::SetCollidableWith(lua_State *f_vm)
{
    ROC::ICollidable *l_colA;
    ROC::ICollidable *l_colB;
    bool l_state;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_colA);
    argStream.ReadElement(l_colB);
    argStream.ReadBoolean(l_state);
    if(!argStream.HasErrors())
    {
        bool l_result = l_colA->SetCollidableWith(l_colB, l_state);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
