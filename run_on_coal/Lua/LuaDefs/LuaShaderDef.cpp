#include "stdafx.h"

#include "Lua/LuaDefs/LuaShaderDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager/LuaManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Drawable.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Shader/ShaderUniform.h"
#include "Lua/ArgReader.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

void ROC::LuaShaderDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Shader", Create);
    LuaUtils::AddClassMethod(f_vm, "setUniformValue", SetUniformValue);
    LuaUtils::AddClassMethod(f_vm, "attach", Attach);
    LuaUtils::AddClassMethod(f_vm, "detach", Detach);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int ROC::LuaShaderDef::Create(lua_State *f_vm)
{
    // element Shader(str vertexPath, str fragmentPath [, str geometryPath = ""])
    std::string l_vsp, l_fsp, l_gsp;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_vsp);
    argStream.ReadText(l_fsp);
    argStream.ReadNextText(l_gsp);
    if(!argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        Shader *l_shader = Core::GetCore()->GetElementManager()->CreateShader(l_vsp, l_fsp, l_gsp);
        l_shader ? argStream.PushElement(l_shader) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaShaderDef::SetUniformValue(lua_State *f_vm)
{
    // bool Shader:setUniformValue(str uniform, var value1 [, ...])
    Shader *l_shader;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        ShaderUniform *l_shaderUniform = l_shader->GetUniform(l_uniform);
        if(l_shaderUniform)
        {
            switch(l_shaderUniform->GetType())
            {
                case ShaderUniform::SUT_Bool:
                {
                    bool l_val;
                    argStream.ReadBoolean(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Bool2:
                {
                    glm::bvec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadBoolean(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Bool3:
                {
                    glm::bvec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadBoolean(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Bool4:
                {
                    glm::bvec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadBoolean(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ShaderUniform::SUT_Int:
                {
                    int l_val;
                    argStream.ReadInteger(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Int2:
                {
                    glm::ivec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Int3:
                {
                    glm::ivec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Int4:
                {
                    glm::ivec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ShaderUniform::SUT_Float:
                {
                    float l_val;
                    argStream.ReadNumber(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Float2:
                {
                    glm::vec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Float3:
                {
                    glm::vec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Float4:
                {
                    glm::vec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_val);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ShaderUniform::SUT_Mat2:
                {
                    glm::mat2 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_mat);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Mat3:
                {
                    glm::mat3 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 9; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_mat);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ShaderUniform::SUT_Mat4:
                {
                    glm::mat4 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 16; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shaderUniform->SetValue(l_mat);
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                default:
                    argStream.PushBoolean(false);
            }
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaShaderDef::Attach(lua_State *f_vm)
{
    // bool Shader:attach(element drawable, str uniform)
    Shader *l_shader;
    Drawable *l_drawable;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->AttachDrawableToShader(l_shader, l_drawable, l_uniform);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaShaderDef::Detach(lua_State *f_vm)
{
    // bool Shader:detach(element drawable)
    Shader *l_shader;
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        bool l_result = Core::GetCore()->GetInheritManager()->DetachDrawableFromShader(l_shader, l_drawable);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
