#pragma once

namespace LuaUtils
{

void AddClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
void AddClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
inline void AddClassFinish(lua_State *f_vm) { lua_pop(f_vm, 1); }

}
