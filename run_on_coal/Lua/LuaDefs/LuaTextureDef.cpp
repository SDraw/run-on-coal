#include "stdafx.h"

#include "Lua/LuaDefs/LuaTextureDef.h"
#include "Lua/LuaDefs/LuaDrawableDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/AsyncManager/AsyncManager.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Elements/Texture.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaFunction.h"
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
    // element Texture(str type, str path [, str filtering, function callback ])
    // element Texture(str type = "cube", str path1, ... , str path6 [, str filtering)
    // element Texture(str type = "cube", str path1, ... , str path6, str filtering, function callback)
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
                for(size_t i = 0U; i < 6U; i++) argStream.ReadText(l_path[6]);
                if(argStream.HasErrors()) l_path.clear();
            } break;
        }
        if(!l_path.empty())
        {
            std::string l_filtering("nearest");
            bool l_compress = false;
            LuaFunction l_callback;

            argStream.ReadNextText(l_filtering);
            int l_filteringType = EnumUtils::ReadEnumVector(l_filtering, g_FilteringTypesTable);
            argStream.ReadNextBoolean(l_compress);
            argStream.ReadNextFunction(l_callback);

            switch(l_path.size())
            {
                case 1U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = Core::GetCore()->GetAsyncManager()->LoadTexture(l_path[0U], l_textureType, l_filteringType, l_compress, l_callback);
                        argStream.PushPointer(l_task);
                    }
                    else
                    {
                        Texture *l_texture = Core::GetCore()->GetElementManager()->CreateTexture(l_path[0U], l_textureType, l_filteringType, l_compress);
                        l_texture ? argStream.PushElement(l_texture) : argStream.PushBoolean(false);
                    }
                } break;
                case 6U:
                {
                    if(l_callback.IsValid())
                    {
                        void *l_task = Core::GetCore()->GetAsyncManager()->LoadTexture(l_path, l_filteringType, l_compress, l_callback);
                        argStream.PushPointer(l_task);
                    }
                    else
                    {
                        Texture *l_texture = Core::GetCore()->GetElementManager()->CreateTexture(l_path, l_filteringType, l_compress);
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
