#include "stdafx.h"

#include "Lua/LuaDefs/LuaDrawableDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager/LuaManager.h"
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
    LuaUtils::AddClassMethod(f_vm, "getSize", GetSize);
    LuaUtils::AddClassMethod(f_vm, "getFiltering", GetFiltering);
    LuaUtils::AddClassMethod(f_vm, "draw", Draw);
    LuaUtils::AddClassMethod(f_vm, "draw3D", Draw3D);
}

int ROC::LuaDrawableDef::IsDrawable(lua_State *f_vm)
{
    // bool isDrawable(element drawable)
    Drawable *l_drawable = nullptr;
    ArgReader argStream(f_vm);
    argStream.ReadNextElement(l_drawable);
    argStream.PushBoolean(l_drawable != nullptr);
    return argStream.GetReturnValue();
}
int ROC::LuaDrawableDef::Draw(lua_State *f_vm)
{
    // bool Drawable:draw(float x, float y, float sizeX, float sizeY [, float rotation = 0, float colorR = 1, float colorG = 1, float colorB = 1, float colorA = 1])
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
        bool l_result = Core::GetCore()->GetRenderManager()->Render(l_drawable, l_pos, l_size, l_rot, l_color);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaDrawableDef::Draw3D(lua_State *f_vm)
{
    // bool Drawable:draw3D(float x, float y, float z, float rotX, float rotY, float rotZ, float width, float height [, bool shading = true, bool depth = true, bool transparency = false, bool doubleSided = false])
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
        bool l_result = Core::GetCore()->GetRenderManager()->Render(l_drawable, l_pos, l_rot, l_size, l_params);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaDrawableDef::GetSize(lua_State *f_vm)
{
    // int int Drawable:getSize()
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        const glm::ivec2 &l_size = l_drawable->GetSize();
        for(int i = 0; i < 2; i++) argStream.PushInteger(l_size[i]);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaDrawableDef::GetFiltering(lua_State *f_vm)
{
    // str Drawable:getFiltering()
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    !argStream.HasErrors() ? argStream.PushText(g_FilteringTypesTable[static_cast<size_t>(l_drawable->GetFiltering())]) : argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
