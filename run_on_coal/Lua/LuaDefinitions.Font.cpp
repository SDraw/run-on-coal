#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Font.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_fontFilteringTypesTable
{
    "nearest", "linear"
};

int fontCreate(lua_State *f_vm)
{
    std::string l_path;
    int l_size;
    std::string l_filter;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_path);
    argStream.ReadInteger(l_size);
    argStream.ReadNextText(l_filter);
    if(!argStream.HasErrors() && !l_path.empty() && l_size > 0)
    {
        int l_filteringType = Utils::ReadEnumVector(g_fontFilteringTypesTable, l_filter);
        if(l_filteringType == -1) l_filteringType = 0;
        Font *l_font = LuaManager::m_corePointer->GetElementManager()->CreateFont_(l_path, l_size, static_cast<unsigned char>(l_filteringType));
        l_font ? lua_pushlightuserdata(f_vm, l_font) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int fontDestroy(lua_State *f_vm)
{
    Font *l_font;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_font), ElementType::FontElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyFont(l_font);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int fontDraw(lua_State *f_vm)
{
    Font *l_font;
    glm::vec2 l_pos;
    std::string l_text;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_font), ElementType::FontElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    argStream.ReadText(l_text);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors() && !l_text.empty())
    {
        sf::String l_utf32Text = sf::String::fromUtf8(l_text.begin(), l_text.end());
        LuaManager::m_corePointer->GetRenderManager()->Render(l_font, l_pos, l_utf32Text, l_color);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}