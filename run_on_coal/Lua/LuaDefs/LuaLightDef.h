#pragma once

namespace ROC
{

class LuaLightDef
{
static int LightCreate(lua_State *f_vm);
static int LightSetParams(lua_State *f_vm);
static int LightGetParams(lua_State *f_vm);
static int LightSetColor(lua_State *f_vm);
static int LightGetColor(lua_State *f_vm);
static int LightSetDirection(lua_State *f_vm);
static int LightGetDirection(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
