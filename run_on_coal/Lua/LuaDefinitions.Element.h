#pragma once

namespace ROC
{
namespace Lua
{

int isElement(lua_State *f_vm);
int elementGetType(lua_State *f_vm);
int elementSetData(lua_State *f_vm);
int elementGetData(lua_State *f_vm);
int elementRemoveData(lua_State *f_vm);
int elementDestroy(lua_State *f_vm);

}
}
