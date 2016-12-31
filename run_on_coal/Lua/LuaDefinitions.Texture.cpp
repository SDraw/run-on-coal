#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Scene/Texture.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Texture.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_textureTypesTable
{
    "rgb", "rgba", "cube"
};
const std::vector<std::string> g_textureFilteringTypesTable
{
    "nearest", "linear"
};

int textureCreate(lua_State *f_vm)
{
    std::string l_type, l_filtering;
    bool l_compress = false;
    int l_argAddCount = 0;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_type);
    if(argStream.ReadNextText(l_filtering)) l_argAddCount++;
    if(argStream.ReadNextBoolean(l_compress)) l_argAddCount++;
    if(!argStream.HasErrors() && !l_type.empty())
    {
        Texture *l_tex = NULL;
        int l_textureType = Utils::ReadEnumVector(g_textureTypesTable, l_type);
        int l_filteringType = Utils::ReadEnumVector(g_textureFilteringTypesTable, l_filtering);
        if(l_filteringType == -1) l_filteringType = 0;
        switch(l_textureType)
        {
            case 0: case 1:
            {
                std::string l_path;
                argStream.ReadText(l_path);
                if(!argStream.HasErrors() && !l_path.empty()) l_tex = LuaManager::m_corePointer->GetElementManager()->CreateTexture(l_path, TEXTURE_TYPE_RGB + l_textureType, static_cast<unsigned char>(l_filteringType), l_compress);
            } break;
            case 2:
            {
                std::vector<std::string> l_path;
                argStream.DecreaseArguments(3 + l_argAddCount);
                argStream.ReadTableTexts(l_path, 6);
                if(!argStream.HasErrors()) l_tex = LuaManager::m_corePointer->GetElementManager()->CreateTexture(l_path, static_cast<unsigned char>(l_filteringType), l_compress);
            } break;
        }
        l_tex ? lua_pushlightuserdata(f_vm, l_tex) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int textureDestroy(lua_State *f_vm)
{
    Texture *l_tex;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_tex), ElementType::TextureElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyTexture(l_tex);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int textureDraw(lua_State *f_vm)
{
    Texture *l_tex;
    glm::vec2 l_pos, l_size;
    float l_rot;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_tex), ElementType::TextureElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::m_corePointer->GetRenderManager()->Render(l_tex, l_pos, l_size, l_rot, l_color);
        lua_pushboolean(f_vm, 1);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}