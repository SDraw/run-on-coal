#pragma once

namespace ROC
{

class LuaTextureDef
{
static int TextureCreate(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
