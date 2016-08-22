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
    ArgReader argStream(f_vm,ROC::LuaManager::m_corePointer);
    std::string l_path;
    argStream.ReadText(l_path);
    if(argStream.HasErrors() || !l_path.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Animation *l_anim = ROC::LuaManager::m_corePointer->GetElementManager()->CreateAnimation(l_path);
    l_anim ? lua_pushlightuserdata(f_vm,l_anim) : lua_pushboolean(f_vm,0);
    return 1;
}
int animationDestroy(lua_State *f_vm)
{
    Animation *l_anim = NULL;
    ArgReader argStream(f_vm,ROC::LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_anim,ElementType::AnimationElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    ROC::LuaManager::m_corePointer->GetElementManager()->DestroyAnimation(l_anim);
    lua_pushboolean(f_vm,1);
    return 1;
}

}
}
