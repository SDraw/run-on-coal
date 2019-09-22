#include "stdafx.h"

#include "Lua/LuaDefs/LuaTextureDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/ITexture.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Module/LuaModule.h"
#include "Module/TaskHandler.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_TextureTypes
{
    "rgb", "rgba", "cube"
};
extern const std::vector<std::string> g_FilteringTypes; // Defined in LuaDrawableDef

void LuaTextureDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Texture", Create);
    LuaDrawableDef::AddHierarchyMethods(f_vm);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaTextureDef::Create(lua_State *f_vm)
{
    // element Texture(str type, str path [, str filtering, function callback ])
    // element Texture(str type = "cube", str path1, ... , str path6 [, str filtering)
    // element Texture(str type = "cube", str path1, ... , str path6, str filtering, function callback)
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_type.empty())
    {
        std::vector<std::string> l_path;
        size_t l_textureType = EnumUtils::ReadEnumVector(l_type, g_TextureTypes);
        switch(l_textureType)
        {
            case ROC::ITexture::TT_RGB: case ROC::ITexture::TT_RGBA:
            {
                l_path.resize(1U);
                argStream.ReadText(l_path[0]);
                if(argStream.HasErrors()) l_path.clear();
            } break;
            case ROC::ITexture::TT_Cubemap:
            {
                l_path.resize(6U);
                for(size_t i = 0U; i < 6U; i++) argStream.ReadText(l_path[i]);
                if(argStream.HasErrors()) l_path.clear();
            } break;
        }
        if(!l_path.empty())
        {
            std::string l_filtering("nearest");
            bool l_compress = false;
            LuaFunction l_callback;

            argStream.ReadNextText(l_filtering);
            size_t l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypes);
            argStream.ReadNextBoolean(l_compress);
            argStream.ReadNextFunction(l_callback);

            switch(l_path.size())
            {
                case 1U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = LuaModule::GetModule()->GetTaskHandler()->CreateTextureTask(l_path[0U], static_cast<unsigned char>(l_textureType), static_cast<unsigned char>(l_filteringType), l_compress, l_callback);
                        argStream.PushPointer(l_task);
                    }
                    else
                    {
                        ROC::ITexture *l_texture = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateITexture(l_path[0U], static_cast<unsigned char>(l_textureType), static_cast<unsigned char>(l_filteringType), l_compress);
                        l_texture ? argStream.PushElement(l_texture) : argStream.PushBoolean(false);
                    }
                } break;
                case 6U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = LuaModule::GetModule()->GetTaskHandler()->CreateTextureTask(l_path, static_cast<unsigned char>(l_filteringType), l_compress, l_callback);
                        argStream.PushPointer(l_task);
                    }
                    else
                    {
                        ROC::ITexture *l_texture = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateITexture(l_path, static_cast<unsigned char>(l_filteringType), l_compress);
                        l_texture ? argStream.PushElement(l_texture) : argStream.PushBoolean(false);
                    }
                } break;
                default:
                    argStream.PushBoolean(false);
                    break;
            }
        }
        else argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
