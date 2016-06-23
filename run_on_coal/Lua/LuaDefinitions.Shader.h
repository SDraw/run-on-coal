#pragma once

namespace ROC 
{
namespace Lua 
{

int shaderCreate(lua_State *f_vm);
int shaderDestroy(lua_State *f_vm);
int shaderGetUniform(lua_State *f_vm);
int shaderSetUniformValue(lua_State *f_vm);

}
}
