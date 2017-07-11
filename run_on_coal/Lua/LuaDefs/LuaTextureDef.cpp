#include "stdafx.h"

#include "Lua/LuaDefs/LuaTextureDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
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
    LuaUtils::lua_registerClass(f_vm, "Texture", TextureCreate);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::lua_registerClassFinish(f_vm);
}

int ROC::LuaTextureDef::TextureCreate(lua_State *f_vm)
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
        int l_textureType = EnumUtils::ReadEnumVector(l_type, g_TextureTypesTable);
        int l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypesTable);
        if(l_filteringType == -1) l_filteringType = 0;
        switch(l_textureType)
        {
            case ROC_TEXTURE_TYPE_RGB: case ROC_TEXTURE_TYPE_RGBA:
            {
                std::string l_path;
                argStream.ReadText(l_path);
                if(!argStream.HasErrors() && !l_path.empty()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, l_textureType, l_filteringType, l_compress);
            } break;
            case ROC_TEXTURE_TYPE_CUBEMAP:
            {
                std::vector<std::string> l_path;
                argStream.ReadVector(l_path, 6);
                if(!argStream.HasErrors()) l_tex = LuaManager::GetCore()->GetElementManager()->CreateTexture(l_path, l_filteringType, l_compress);
            } break;
        }
        l_tex ? argStream.PushElement(l_tex) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
