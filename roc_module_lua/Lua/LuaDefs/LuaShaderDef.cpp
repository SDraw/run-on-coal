#include "stdafx.h"

#include "Lua/LuaDefs/LuaShaderDef.h"

#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IShader.h"
#include "Interfaces/IShaderUniform.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/LuaUtils.h"

void LuaShaderDef::Init(lua_State *f_vm)
{
    LuaUtils::AddClass(f_vm, "Shader", Create);
    LuaUtils::AddClassMethod(f_vm, "setUniformValue", SetUniformValue);
    LuaUtils::AddClassMethod(f_vm, "attach", Attach);
    LuaUtils::AddClassMethod(f_vm, "detach", Detach);
    LuaElementDef::AddHierarchyMethods(f_vm);
    LuaUtils::AddClassFinish(f_vm);
}

int LuaShaderDef::Create(lua_State *f_vm)
{
    // element Shader(str vertexPath, str fragmentPath [, str geometryPath = ""])
    std::string l_vsp, l_fsp, l_gsp;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_vsp);
    argStream.ReadText(l_fsp);
    argStream.ReadNextText(l_gsp);
    if(!argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        ROC::IShader *l_shader = LuaModule::GetModule()->GetEngineCore()->GetIElementManager()->CreateIShader(l_vsp, l_fsp, l_gsp);
        l_shader ? argStream.PushElement(l_shader) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaShaderDef::SetUniformValue(lua_State *f_vm)
{
    // bool Shader:setUniformValue(str uniform, var value1 [, ...])
    ROC::IShader *l_shader;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        ROC::IShaderUniform *l_shaderUniform = l_shader->GetIUniform(l_uniform);
        if(l_shaderUniform)
        {
            switch(l_shaderUniform->GetType())
            {
                case ROC::IShaderUniform::SUT_Bool:
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
                case ROC::IShaderUniform::SUT_Bool2:
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
                case ROC::IShaderUniform::SUT_Bool3:
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
                case ROC::IShaderUniform::SUT_Bool4:
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

                case ROC::IShaderUniform::SUT_Int:
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
                case ROC::IShaderUniform::SUT_Int2:
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
                case ROC::IShaderUniform::SUT_Int3:
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
                case ROC::IShaderUniform::SUT_Int4:
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

                case ROC::IShaderUniform::SUT_Float:
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
                case ROC::IShaderUniform::SUT_Float2:
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
                case ROC::IShaderUniform::SUT_Float3:
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
                case ROC::IShaderUniform::SUT_Float4:
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

                case ROC::IShaderUniform::SUT_Mat2:
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
                case ROC::IShaderUniform::SUT_Mat3:
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
                case ROC::IShaderUniform::SUT_Mat4:
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
                    break;
            }
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaShaderDef::Attach(lua_State *f_vm)
{
    // bool Shader:attach(element drawable, str uniform)
    ROC::IShader *l_shader;
    ROC::IDrawable *l_drawable;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        bool l_result = l_shader->Attach(l_drawable, l_uniform);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int LuaShaderDef::Detach(lua_State *f_vm)
{
    // bool Shader:detach(element drawable)
    ROC::IShader *l_shader;
    ROC::IDrawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        bool l_result = l_shader->Detach(l_drawable);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
