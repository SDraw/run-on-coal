#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Elements/Animation/Animation.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Animation.h"

namespace ROC
{
namespace Lua
{

int animationCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Animation *l_anim = ROC::LuaManager::GetCore()->GetElementManager()->CreateAnimation(l_path);
        l_anim ? argStream.PushPointer(l_anim) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int animationDestroy(lua_State *f_vm)
{
    Animation *l_anim = NULL;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_anim);
    if(!argStream.HasErrors())
    {
        ROC::LuaManager::GetCore()->GetElementManager()->DestroyElement(l_anim);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}