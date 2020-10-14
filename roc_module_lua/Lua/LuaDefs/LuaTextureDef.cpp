#include "stdafx.h"

#include "Lua/LuaDefs/LuaTextureDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Core/Core.h"
#include "Core/TaskHandler.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_textureTypes
{
    "rgb", "rgba", "cube"
};
extern const std::vector<std::string> g_filteringTypes; // Defined in LuaDrawableDef

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
    // element Texture(str type = "cube", str path1, ... , str path6 [, str filtering, function callback])
    std::string l_type;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_type);
    if(!l_argStream.HasErrors() && !l_type.empty())
    {
        std::vector<std::string> l_path;
        size_t l_textureType = EnumUtils::ReadEnumVector(l_type, g_textureTypes);
        switch(l_textureType)
        {
            case ROC::ITexture::TT_RGB: case ROC::ITexture::TT_RGBA:
            {
                l_path.resize(1U);
                l_argStream.ReadText(l_path[0]);
                if(l_argStream.HasErrors()) l_path.clear();
            } break;
            case ROC::ITexture::TT_Cubemap:
            {
                l_path.resize(6U);
                for(size_t i = 0U; i < 6U; i++) l_argStream.ReadText(l_path[i]);
                if(l_argStream.HasErrors()) l_path.clear();
            } break;
        }
        if(!l_path.empty())
        {
            std::string l_filtering("nearest");
            bool l_compress = false;
            LuaFunction l_callback;

            l_argStream.ReadNextText(l_filtering);
            size_t l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_filteringTypes);
            l_argStream.ReadNextBoolean(l_compress);
            l_argStream.ReadNextFunction(l_callback);

            switch(l_path.size())
            {
                case 1U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = Core::GetInstance()->GetTaskHandler()->CreateTextureTask(l_path[0U], static_cast<unsigned char>(l_textureType), static_cast<unsigned char>(l_filteringType), l_compress, l_callback);
                        l_argStream.PushPointer(l_task);
                    }
                    else
                    {
                        ROC::ITexture *l_texture = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateITexture(l_path[0U], static_cast<unsigned char>(l_textureType), static_cast<unsigned char>(l_filteringType), l_compress);
                        l_texture ? l_argStream.PushElement(l_texture) : l_argStream.PushBoolean(false);
                    }
                } break;
                case 6U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = Core::GetInstance()->GetTaskHandler()->CreateTextureTask(l_path, static_cast<unsigned char>(l_filteringType), l_compress, l_callback);
                        l_argStream.PushPointer(l_task);
                    }
                    else
                    {
                        ROC::ITexture *l_texture = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateITexture(l_path, static_cast<unsigned char>(l_filteringType), l_compress);
                        l_texture ? l_argStream.PushElement(l_texture) : l_argStream.PushBoolean(false);
                    }
                } break;
                default:
                    l_argStream.PushBoolean(false);
                    break;
            }
        }
        else l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
