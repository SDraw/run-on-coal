#pragma once

namespace ROC
{
namespace Lua
{

int fileCreate(lua_State *f_vm);
int fileOpen(lua_State *f_vm);
int fileRead(lua_State *f_vm);
int fileWrite(lua_State *f_vm);
int fileGetSize(lua_State *f_vm);
int fileSetPosition(lua_State *f_vm);
int fileGetPosition(lua_State *f_vm);
int fileFlush(lua_State *f_vm);
int fileGetPath(lua_State *f_vm);
int fileIsEOF(lua_State *f_vm);
int fileDelete(lua_State *f_vm);
int fileRename(lua_State *f_vm);

}
}
