#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/LuaManager.h"
#include "Managers/SfmlManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Sfml.h"

namespace ROC
{
namespace Lua
{

int getTime(lua_State *f_vm)
{
    ArgReader argStream(f_vm);
    argStream.PushNumber(LuaManager::GetCore()->GetSfmlManager()->GetTime());
    return argStream.GetReturnValue();
}

}
}
