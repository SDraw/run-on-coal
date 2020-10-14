#include "stdafx.h"

#include "Lua/LuaDefs/LuaShaderDef.h"

#include "Lua/LuaArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Core/Core.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_shaderUniformTypes
{
    "float", "float2", "float3", "float4",
    "int", "int2", "int3", "int4",
    "uint", "uint2", "uint3", "uint4",
    "double", "double2", "double3", "double4",
    "mat2", "mat3", "mat4",
    "dmat2", "dmat3", "dmat4"
};

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
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadText(l_vsp);
    l_argStream.ReadText(l_fsp);
    l_argStream.ReadNextText(l_gsp);
    if(!l_argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        ROC::IShader *l_shader = Core::GetInstance()->GetEngineICore()->GetIElementManager()->CreateIShader(l_vsp, l_fsp, l_gsp);
        l_shader ? l_argStream.PushElement(l_shader) : l_argStream.PushBoolean(false);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaShaderDef::SetUniformValue(lua_State *f_vm)
{
    // bool Shader:setUniformValue(str uniform, str uniformType, var value1 [, ...])
    ROC::IShader *l_shader;
    std::string l_uniform;
    std::string l_uniformType;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_shader);
    l_argStream.ReadText(l_uniform);
    l_argStream.ReadText(l_uniformType);
    if(!l_argStream.HasErrors() && !l_uniform.empty() && !l_uniformType.empty())
    {
        size_t l_uniformTypeIndex = EnumUtils::ReadEnumVector(l_uniformType, g_shaderUniformTypes);
        if(l_uniformTypeIndex != std::numeric_limits<size_t>::max())
        {
            switch(l_uniformTypeIndex)
            {
                case ROC::IShader::SUT_Int:
                {
                    int l_val;
                    l_argStream.ReadInteger(l_val);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(int));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int2:
                {
                    glm::ivec2 l_val;
                    for(int i = 0; i < 2; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int3:
                {
                    glm::ivec3 l_val;
                    for(int i = 0; i < 3; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int4:
                {
                    glm::ivec4 l_val;
                    for(int i = 0; i < 4; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_UInt:
                {
                    unsigned int l_val;
                    l_argStream.ReadInteger(l_val);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(unsigned int));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt2:
                {
                    glm::uvec2 l_val;
                    for(int i = 0; i < 2; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt3:
                {
                    glm::uvec3 l_val;
                    for(int i = 0; i < 3; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt4:
                {
                    glm::uvec4 l_val;
                    for(int i = 0; i < 4; i++) l_argStream.ReadInteger(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                
                case ROC::IShader::SUT_Float:
                {
                    float l_val;
                    l_argStream.ReadNumber(l_val);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(float));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float2:
                {
                    glm::vec2 l_val;
                    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float3:
                {
                    glm::vec3 l_val;
                    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float4:
                {
                    glm::vec4 l_val;
                    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Double:
                {
                    double l_val;
                    l_argStream.ReadNumber(l_val);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(double));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double2:
                {
                    glm::dvec2 l_val;
                    for(int i = 0; i < 2; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double3:
                {
                    glm::dvec3 l_val;
                    for(int i = 0; i < 3; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double4:
                {
                    glm::dvec4 l_val;
                    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_val[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_FloatMat2:
                {
                    glm::mat2 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_FloatMat3:
                {
                    glm::mat3 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 9; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_FloatMat4:
                {
                    glm::mat4 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 16; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_DoubleMat2:
                {
                    glm::dmat2 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 4; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat2));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_DoubleMat3:
                {
                    glm::dmat3 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 9; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat3));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_DoubleMat4:
                {
                    glm::dmat4 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 16; i++) l_argStream.ReadNumber(l_matPtr[i]);
                    if(!l_argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat4));
                        l_argStream.PushBoolean(l_result);
                    }
                    else l_argStream.PushBoolean(false);
                } break;

                default:
                    l_argStream.PushBoolean(false);
                    break;
            }
        }
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaShaderDef::Attach(lua_State *f_vm)
{
    // bool Shader:attach(element drawable, str uniform)
    ROC::IShader *l_shader;
    ROC::IDrawable *l_drawable;
    std::string l_uniform;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_shader);
    l_argStream.ReadElement(l_drawable);
    l_argStream.ReadText(l_uniform);
    if(!l_argStream.HasErrors() && !l_uniform.empty())
    {
        bool l_result = l_shader->Attach(l_drawable, l_uniform);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}

int LuaShaderDef::Detach(lua_State *f_vm)
{
    // bool Shader:detach(element drawable)
    ROC::IShader *l_shader;
    ROC::IDrawable *l_drawable;
    LuaArgReader l_argStream(f_vm);
    l_argStream.ReadElement(l_shader);
    l_argStream.ReadElement(l_drawable);
    if(!l_argStream.HasErrors())
    {
        bool l_result = l_shader->Detach(l_drawable);
        l_argStream.PushBoolean(l_result);
    }
    else l_argStream.PushBoolean(false);
    return l_argStream.GetReturnValue();
}
