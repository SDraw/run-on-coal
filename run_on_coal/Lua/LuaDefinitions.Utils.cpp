#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/LogManager.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Utils.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{
int disabledFunction(lua_State *f_vm)
{
    lua_pushboolean(f_vm, 0);
    return 1;
}

int logPrint(lua_State *f_vm)
{
    std::string l_text;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_text);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    LuaManager::m_corePointer->GetLogManager()->Log(l_text);
    lua_pushboolean(f_vm, 1);
    return 1;
}
int isElement(lua_State *f_vm)
{
    void *l_pointer = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadPointer(&l_pointer);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool l_result = LuaManager::m_corePointer->GetMemoryManager()->IsValidMemoryPointer(l_pointer);
    lua_pushboolean(f_vm, l_result);
    return 1;
}

int getElementType(lua_State *f_vm)
{
    void *l_pointer = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadPointer(&l_pointer);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    int l_type = LuaManager::m_corePointer->GetMemoryManager()->GetMemoryPointerType(l_pointer);
    std::string l_text;
    switch(l_type)
    {
        case ElementType::GeometryElement:
            l_text.append("geometry");
            break;
        case ElementType::ModelElement:
            l_text.append("model");
            break;
        case ElementType::AnimationElement:
            l_text.append("animation");
            break;
        case ElementType::SceneElement:
            l_text.append("scene");
            break;
        case ElementType::CameraElement:
            l_text.append("camera");
            break;
        case ElementType::LightElement:
            l_text.append("light");
            break;
        case ElementType::RenderTargetElement:
            l_text.append("rendertarget");
            break;
        case ElementType::ShaderElement:
            l_text.append("shader");
            break;
        case ElementType::SoundElement:
            l_text.append("sound");
            break;
        case ElementType::TextureElement:
            l_text.append("texture");
            break;
        case ElementType::FontElement:
            l_text.append("font");
            break;
        case ElementType::FileElement:
            l_text.append("file");
            break;
        case ElementType::CollisionElement:
            l_text.append("collision");
            break;
        default:
            l_text.append("invalid");
            break;
    }
    lua_pushstring(f_vm, l_text.c_str());
    return 1;
}
}
}