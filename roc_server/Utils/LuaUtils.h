#pragma once

namespace LuaUtils
{

void AddClass(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
void AddClassMethod(lua_State *f_vm, const char *f_name, lua_CFunction f_func);
inline void AddClassFinish(lua_State *f_vm) { lua_pop(f_vm, 1); }
void PushElementInMetatable(lua_State *f_vm, const char *f_mt, void *f_ptr, const char *f_class);

}
