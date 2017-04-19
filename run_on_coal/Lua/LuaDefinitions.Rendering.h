#pragma once

namespace ROC
{
namespace Lua
{

int setActiveScene(lua_State *f_vm);
int setActiveShader(lua_State *f_vm);
int setRenderTarget(lua_State *f_vm);
int clearRenderArea(lua_State *f_vm);
int setClearColor(lua_State *f_vm);
int setRenderArea(lua_State *f_vm);
int setPolygonMode(lua_State *f_vm);

}
}
