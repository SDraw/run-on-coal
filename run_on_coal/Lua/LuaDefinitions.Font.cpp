#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Font.h"

namespace ROC
{
namespace Lua 
{

int fontCreate(lua_State *f_vm)
{
    std::string l_path;
    LUA_INTEGER l_size;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadInteger(l_size);
    if(argStream.HasErrors() || !l_path.length() || l_size < 1)
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    Font *l_font = LuaManager::m_corePointer->GetElementManager()->CreateFont_(l_path,static_cast<int>(l_size));
    l_font ? lua_pushlightuserdata(f_vm,l_font) : lua_pushboolean(f_vm,0);
    return 1;
}
int fontDestroy(lua_State *f_vm)
{
    Font *l_font;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_font,ElementType::FontElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyFont(l_font);
    lua_pushboolean(f_vm,result);
    return 1;
}
int fontDraw(lua_State *f_vm)
{
    Font *l_font;
    lua_Number l_pos[2];
    lua_Number l_color[4] = { 1.0,1.0,1.0,1.0 };
    std::string l_text;
    ArgReader argStream(f_vm,LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_font,ElementType::FontElement);
    for(int i=0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    argStream.ReadText(l_text);
    for(int i=0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(argStream.HasErrors() || !l_text.length())
    {
        lua_pushboolean(f_vm,0);
        return 1;
    }
    glm::vec2 l_vPos(l_pos[0],l_pos[1]);
    glm::vec4 l_vColor(l_color[0],l_color[1],l_color[2],l_color[3]);
    std::wstring l_converted;
    utf8::utf8to32(l_text.begin(),l_text.end(),std::back_inserter(l_converted));
    LuaManager::m_corePointer->GetRenderManager()->Render(l_font,l_vPos,l_converted,l_vColor);
    lua_pushboolean(f_vm,1);
    return 1;
}

}
}
