#include "stdafx.h"

#include "Lua/LuaDefs/LuaDrawableDef.h"

#include "Lua/LuaArgReader.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

extern const std::vector<std::string> g_filteringTypes
{
    "nearest", "linear"
};

void LuaDrawableDef::Init(lua_State *f_vm)
{
    lua_register(f_vm, "isDrawable", IsDrawable);
}

void LuaDrawableDef::AddHierarchyMethods(lua_State *f_vm)
{
    LuaUtils::AddClassMethod(f_vm, "getSize", GetSize);
    LuaUtils::AddClassMethod(f_vm, "getFiltering", GetFiltering);
    LuaUtils::AddClassMethod(f_vm, "draw", Draw);
    LuaUtils::AddClassMethod(f_vm, "draw3D", Draw3D);
}

int LuaDrawableDef::IsDrawable(lua_State *f_vm)
{
    // bool isDrawable(element drawable)
    ROC::IDrawable *l_drawable = nullptr;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadNextElement(l_drawable);
    l_argStream.PushBoolean(l_drawable != nullptr);
    return l_argStream.GetReturnValue();
}

int LuaDrawableDef::Draw(lua_State *f_vm)
{
    // bool Drawable:draw(float x, float y, float sizeX, float sizeY [, float rotation = 0, float colorR = 1, float colorG = 1, float colorB = 1, float colorA = 1, str layer = "screen"])
    ROC::IDrawable *l_drawable;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    std::string l_layer("screen");
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_drawable);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_size[i]);
    l_argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) l_argStream.ReadNextNumber(l_color[i]);
    l_argStream.ReadNextText(l_layer);
    if(!l_argStream.HasErrors())
    {
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->Render(l_drawable, l_pos, l_size, l_rot, l_color, l_layer);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaDrawableDef::Draw3D(lua_State *f_vm)
{
    // bool Drawable:draw3D(float x, float y, float z, float rotX, float rotY, float rotZ, float width, float height [, str layer = "default", bool shading = true, bool depth = true, bool transparency = false, bool doubleSided = false])
    ROC::IDrawable *l_drawable;
    glm::vec3 l_pos;
    glm::vec3 l_rot;
    glm::vec2 l_size;
    std::string l_layer("default");
    glm::bvec4 l_params(true, true, false, false);
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_drawable);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_rot[i]);
    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_size[i]);
    l_argStream.ReadNextText(l_layer);
    for(int i = 0; i < 4; i++) l_argStream.ReadNextBoolean(l_params[i]);
    if(!l_argStream.HasErrors())
    {
        glm::quat l_rotQuat(l_rot);
        bool l_result = LuaModule::GetModule()->GetEngineCore()->GetIRenderManager()->Render(l_drawable, l_pos, l_rot, l_size, l_layer, l_params);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaDrawableDef::GetSize(lua_State *f_vm)
{
    // int int Drawable:getSize()
    ROC::IDrawable *l_drawable;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_drawable);
    if(!l_argStream.HasErrors())
    {
        const glm::ivec2 &l_size = l_drawable->GetSize();
        for(int i = 0; i < 2; i++) l_argStream.PushInteger(l_size[i]);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaDrawableDef::GetFiltering(lua_State *f_vm)
{
    // str Drawable:getFiltering()
    ROC::IDrawable *l_drawable;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_drawable);
    !l_argStream.HasErrors() ? l_argStream.PushText(g_filteringTypes[l_drawable->GetFiltering()]) : l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
