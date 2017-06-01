#pragma once

namespace ROC
{
namespace Lua
{

int shaderCreate(lua_State *f_vm);
int shaderSetUniformValue(lua_State *f_vm);
int shaderAttachDrawable(lua_State *f_vm);
int shaderDetachDrawable(lua_State *f_vm);


}
}
