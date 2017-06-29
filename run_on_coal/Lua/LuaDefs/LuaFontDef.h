#pragma once

namespace ROC
{

class LuaFontDef
{
    static int FontCreate(lua_State *f_vm);
    static int FontDraw(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;
};

}
