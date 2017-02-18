#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Texture.h"
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
    ArgReader argStream(f_vm);
    argStream.ReadText(l_type);
    argStream.ReadNextText(l_filtering);
    argStream.ReadNextBoolean(l_compress);
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
                if(!argStream.HasErrors() && !l_path.empty()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, TEXTURE_TYPE_RGB + l_textureType, static_cast<unsigned char>(l_filteringType), l_compress);
            } break;
            case 2:
            {
                std::vector<std::string> l_path;
                argStream.ReadTableTexts(l_path, 6);
                if(!argStream.HasErrors()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, static_cast<unsigned char>(l_filteringType), l_compress);
            } break;
        }
        l_tex ? argStream.PushPointer(l_tex) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int textureDestroy(lua_State *f_vm)
{
    Texture *l_tex;
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_tex), ElementType::TextureElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetElementManager()->DestroyTexture(l_tex);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int textureDraw(lua_State *f_vm)
{
    Texture *l_tex;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_tex), ElementType::TextureElement);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->Render(l_tex, l_pos, l_size, l_rot, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
