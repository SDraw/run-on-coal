#pragma once

namespace ROC
{

class LuaModelDef final
{
    static int Create(lua_State *f_vm);
    static int GetGeometry(lua_State *f_vm);
    static int SetPosition(lua_State *f_vm);
    static int GetPosition(lua_State *f_vm);
    static int SetRotation(lua_State *f_vm);
    static int GetRotation(lua_State *f_vm);
    static int SetScale(lua_State *f_vm);
    static int GetScale(lua_State *f_vm);
    static int GetMatrix(lua_State *f_vm);
    static int Attach(lua_State *f_vm);
    static int Detach(lua_State *f_vm);
    static int GetParent(lua_State *f_vm);
    static int SetAnimation(lua_State *f_vm);
    static int GetAnimation(lua_State *f_vm);
    static int RemoveAnimation(lua_State *f_vm);
    static int PlayAnimation(lua_State *f_vm);
    static int PauseAnimation(lua_State *f_vm);
    static int ResetAnimation(lua_State *f_vm);
    static int SetAnimationProperty(lua_State *f_vm);
    static int GetAnimationProperty(lua_State *f_vm);
    static int SetCollision(lua_State *f_vm);
    static int GetCollision(lua_State *f_vm);
    static int RemoveCollision(lua_State *f_vm);
    static int SetCollidable(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
