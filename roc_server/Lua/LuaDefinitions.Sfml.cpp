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
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.PushNumber(LuaManager::m_corePointer->GetSfmlManager()->GetTime());
    return argStream.GetReturnValue();
}

}
}
