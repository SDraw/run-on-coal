#include "stdafx.h"

#include "Lua/LuaDefs/LuaDrawableDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Drawable.h"
#include "Lua/ArgReader.h"
#include "Utils/LuaUtils.h"

namespace ROC
{

extern const std::vector<std::string> g_FilteringTypesTable;

}

void ROC::LuaDrawableDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isDrawable", IsDrawable);
}
void ROC::LuaDrawableDef::AddHierarchyMethods(lua_State *f_vm)
{
    LuaUtils::lua_registerClassMethod(f_vm, "getSize", DrawableGetSize);
    LuaUtils::lua_registerClassMethod(f_vm, "getFiltering", DrawableGetFiltering);
    LuaUtils::lua_registerClassMethod(f_vm, "draw", DrawableDraw);
    LuaUtils::lua_registerClassMethod(f_vm, "draw3D", DrawableDraw3D);
}

int ROC::LuaDrawableDef::IsDrawable(lua_State *f_vm)
{
    Drawable *l_drawable = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_drawable);
    argStream.PushBoolean(l_drawable != nullptr);
    return argStream.GetReturnValue();
}
int ROC::LuaDrawableDef::DrawableDraw(lua_State *f_vm)
{
    Drawable *l_drawable;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->Render(l_drawable, l_pos, l_size, l_rot, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaDrawableDef::DrawableDraw3D(lua_State *f_vm)
{
    Drawable *l_drawable;
    glm::vec3 l_pos;
    glm::vec3 l_rot;
    glm::vec2 l_size;
    glm::bvec4 l_params(true, true, false, false);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_rot[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    for(int i = 0; i < 4; i++) argStream.ReadNextBoolean(l_params[i]);
    if(!argStream.HasErrors())
    {
        glm::quat l_rotQuat(l_rot);
        LuaManager::GetCore()->GetRenderManager()->Render(l_drawable, l_pos, l_rot, l_size, l_params);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaDrawableDef::DrawableGetSize(lua_State *f_vm)
{
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        glm::ivec2 l_size;
        l_drawable->GetSize(l_size);
        argStream.PushInteger(l_size.x);
        argStream.PushInteger(l_size.y);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaDrawableDef::DrawableGetFiltering(lua_State *f_vm)
{
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    !argStream.HasErrors() ? argStream.PushText(g_FilteringTypesTable[l_drawable->GetFiltering()]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
