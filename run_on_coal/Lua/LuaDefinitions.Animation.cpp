#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Animation.h"

namespace ROC
{
namespace Lua
{
int animationCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm, ROC::LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Animation *l_anim = ROC::LuaManager::m_corePointer->GetElementManager()->CreateAnimation(l_path);
        l_anim ? lua_pushlightuserdata(f_vm, l_anim) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int animationDestroy(lua_State *f_vm)
{
    Animation *l_anim = NULL;
    ArgReader argStream(f_vm, ROC::LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_anim), ElementType::AnimationElement);
    if(!argStream.HasErrors())
    {
        ROC::LuaManager::m_corePointer->GetElementManager()->DestroyAnimation(l_anim);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
}
}