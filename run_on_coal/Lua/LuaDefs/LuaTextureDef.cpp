#include "stdafx.h"

#include "Lua/LuaDefs/LuaTextureDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Elements/Texture.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"
#include "Utils/EnumUtils.h"

namespace ROC
{

const std::vector<std::string> g_TextureTypesTable
{
    "rgb", "rgba", "cube"
};
extern const std::vector<std::string> g_FilteringTypesTable;

}

void ROC::LuaTextureDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Texture", Create);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaTextureDef::Create(lua_State *f_vm)
{
    // element Texture(str type, str path, str filtering)
    // element Texture(str type = "cube", str path1, ... , str path6, str filtering)
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_type.empty())
    {
        std::vector<std::string> l_path;
        int l_textureType = EnumUtils::ReadEnumVector(l_type, g_TextureTypesTable);
        switch(l_textureType)
        {
            case Texture::TT_RGB: case Texture::TT_RGBA:
            {
                l_path.resize(1U);
                argStream.ReadText(l_path[0]);
                if(argStream.HasErrors()) l_path.clear();
            } break;
            case Texture::TT_Cubemap:
            {
                l_path.resize(6U);
                for(int i = 0; i < 6; i++) argStream.ReadText(l_path[6]);
                if(argStream.HasErrors()) l_path.clear();
            } break;
        }
        if(!l_path.empty())
        {
            std::string l_filtering;
            argStream.ReadNextText(l_filtering);
            int l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypesTable);

            bool l_compress = false;
            argStream.ReadNextBoolean(l_compress);

            Texture *l_texture = nullptr;
            if(l_path.size() == 1U) l_texture = Core::GetCore()->GetElementManager()->CreateTexture(l_path[0], l_textureType, l_filteringType, l_compress);
            else if(l_path.size() == 6U)  l_texture = Core::GetCore()->GetElementManager()->CreateTexture(l_path, l_filteringType, l_compress);
            l_texture ? argStream.PushElement(l_texture) : argStream.PushBoolean(false);
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
