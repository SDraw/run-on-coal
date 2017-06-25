#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
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
        Texture *l_tex = nullptr;
        int l_textureType = Utils::Enum::ReadEnumVector(g_textureTypesTable, l_type);
        int l_filteringType = Utils::Enum::ReadEnumVector(g_textureFilteringTypesTable, l_filtering);
        if(l_filteringType == -1) l_filteringType = 0;
        switch(l_textureType)
        {
            case 0: case 1:
            {
                std::string l_path;
                argStream.ReadText(l_path);
                if(!argStream.HasErrors() && !l_path.empty()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, l_textureType, l_filteringType, l_compress);
            } break;
            case 2:
            {
                std::vector<std::string> l_path;
                argStream.ReadVector(l_path, 6);
                if(!argStream.HasErrors()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, l_filteringType, l_compress);
            } break;
        }
        l_tex ? argStream.PushPointer(l_tex) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int textureGetSize(lua_State *f_vm)
{
    Texture *l_texture;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_texture);
    if(!argStream.HasErrors())
    {
        glm::ivec2 l_size;
        l_texture->GetSize(l_size);
        argStream.PushInteger(l_size.x);
        argStream.PushInteger(l_size.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int textureGetFiltering(lua_State *f_vm)
{
    Texture *l_texture;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_texture);
    !argStream.HasErrors() ? argStream.PushText(g_textureFilteringTypesTable[l_texture->GetFiltering()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
