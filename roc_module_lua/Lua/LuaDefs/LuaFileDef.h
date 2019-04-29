#pragma once

class LuaFileDef final
{
    static int CreateOpen(lua_State *f_vm);
    static int Read(lua_State *f_vm);
    static int Write(lua_State *f_vm);
    static int GetSize(lua_State *f_vm);
    static int SetPosition(lua_State *f_vm);
    static int GetPosition(lua_State *f_vm);
    static int Flush(lua_State *f_vm);
    static int GetPath(lua_State *f_vm);
    static int IsEOF(lua_State *f_vm);
    static int Delete(lua_State *f_vm);
    static int Rename(lua_State *f_vm);
protected:
    static void Init(lua_State *f_vm);

    friend class LuaVM;

};
