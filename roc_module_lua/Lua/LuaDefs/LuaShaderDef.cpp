#include "stdafx.h"

#include "Lua/LuaDefs/LuaShaderDef.h"

#include "Lua/ArgReader.h"
#include "Lua/LuaDefs/LuaElementDef.h"
#include "Module/LuaModule.h"
#include "Utils/EnumUtils.h"
#include "Utils/LuaUtils.h"

const std::vector<std::string> g_ShaderUniformTypes
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
    // bool Shader:setUniformValue(str uniform, str uniformType, var value1 [, ...])
    ROC::IShader *l_shader;
    std::string l_uniform;
    std::string l_uniformType;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadText(l_uniform);
    argStream.ReadText(l_uniformType);
    if(!argStream.HasErrors() && !l_uniform.empty() && !l_uniformType.empty())
    {
        size_t l_uniformTypeIndex = EnumUtils::ReadEnumVector(l_uniformType, g_ShaderUniformTypes);
        if(l_uniformTypeIndex != std::numeric_limits<size_t>::max())
        {
            switch(l_uniformTypeIndex)
            {
                case ROC::IShader::SUT_Int:
                {
                    int l_val;
                    argStream.ReadInteger(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(int));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int2:
                {
                    glm::ivec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int3:
                {
                    glm::ivec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Int4:
                {
                    glm::ivec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::ivec4));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_UInt:
                {
                    unsigned int l_val;
                    argStream.ReadInteger(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(unsigned int));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt2:
                {
                    glm::uvec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt3:
                {
                    glm::uvec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_UInt4:
                {
                    glm::uvec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::uvec4));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                
                case ROC::IShader::SUT_Float:
                {
                    float l_val;
                    argStream.ReadNumber(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(float));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float2:
                {
                    glm::vec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float3:
                {
                    glm::vec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Float4:
                {
                    glm::vec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::vec4));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_Double:
                {
                    double l_val;
                    argStream.ReadNumber(l_val);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(double));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double2:
                {
                    glm::dvec2 l_val;
                    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double3:
                {
                    glm::dvec3 l_val;
                    for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_Double4:
                {
                    glm::dvec4 l_val;
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), &l_val, sizeof(glm::dvec4));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_FloatMat2:
                {
                    glm::mat2 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_FloatMat3:
                {
                    glm::mat3 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 9; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_FloatMat4:
                {
                    glm::mat4 l_mat;
                    float *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 16; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::mat4));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;

                case ROC::IShader::SUT_DoubleMat2:
                {
                    glm::dmat2 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 4; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat2));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_DoubleMat3:
                {
                    glm::dmat3 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 9; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat3));
                        argStream.PushBoolean(l_result);
                    }
                    else argStream.PushBoolean(false);
                } break;
                case ROC::IShader::SUT_DoubleMat4:
                {
                    glm::dmat4 l_mat;
                    double *l_matPtr = glm::value_ptr(l_mat);
                    for(int i = 0; i < 16; i++) argStream.ReadNumber(l_matPtr[i]);
                    if(!argStream.HasErrors())
                    {
                        bool l_result = l_shader->SetUniformValue(l_uniform, static_cast<ROC::IShader::ShaderUniformType>(l_uniformTypeIndex), l_matPtr, sizeof(glm::dmat4));
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
