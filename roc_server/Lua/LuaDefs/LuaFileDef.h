#pragma once

namespace ROC
{

class LuaFileDef final
{
    static int FileCreateOpen(lua_State *f_vm);
    static int FileRead(lua_State *f_vm);
    static int FileWrite(lua_State *f_vm);
    static int FileGetSize(lua_State *f_vm);
    static int FileSetPosition(lua_State *f_vm);
    static int FileGetPosition(lua_State *f_vm);
    static int FileFlush(lua_State *f_vm);
    static int FileGetPath(lua_State *f_vm);
    static int FileIsEOF(lua_State *f_vm);
    static int FileDelete(lua_State *f_vm);
    static int FileRename(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaManager;

};

}
