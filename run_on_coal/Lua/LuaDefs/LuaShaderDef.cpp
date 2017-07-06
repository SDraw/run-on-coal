#include "stdafx.h"

#include "Lua/LuaDefs/LuaShaderDef.h"
#include "Lua/LuaDefs/LuaElementDef.h"

#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Drawable.h"
#include "Elements/Shader.h"
#include "Lua/ArgReader.h"
#include "Utils/Utils.h"

#define ROC_SHADER_UNIFORM_UINT 0
#define ROC_SHADER_UNIFORM_UVEC2 1
#define ROC_SHADER_UNIFORM_UVEC3 2
#define ROC_SHADER_UNIFORM_UVEC4 3
#define ROC_SHADER_UNIFORM_INT 4
#define ROC_SHADER_UNIFORM_IVEC2 5
#define ROC_SHADER_UNIFORM_IVEC3 6
#define ROC_SHADER_UNIFORM_IVEC4 7
#define ROC_SHADER_UNIFORM_FLOAT 8
#define ROC_SHADER_UNIFORM_VEC2 9
#define ROC_SHADER_UNIFORM_VEC3 10
#define ROC_SHADER_UNIFORM_VEC4 11
#define ROC_SHADER_UNIFORM_DOUBLE 12
#define ROC_SHADER_UNIFORM_DVEC2 13
#define ROC_SHADER_UNIFORM_DVEC3 14
#define ROC_SHADER_UNIFORM_DVEC4 15
#define ROC_SHADER_UNIFORM_MAT2 16
#define ROC_SHADER_UNIFORM_MAT3 17
#define ROC_SHADER_UNIFORM_MAT4 18

namespace ROC
{

const std::vector<std::string> g_UniformTypesTable
{
    "uint", "uvec2", "uvec3", "uvec4",
    "int", "ivec2", "ivec3", "ivec4",
    "float", "vec2", "vec3", "vec4",
    "double", "dvec2", "dvec3", "dvec4",
    "mat2", "mat3", "mat4"
};

}

void ROC::LuaShaderDef::Init(lua_State *f_vm)
{
    Utils::Lua::lua_registerClass(f_vm, "Shader", ShaderCreate);
    Utils::Lua::lua_registerClassMethod(f_vm, "setUniformValue", ShaderSetUniformValue);
    Utils::Lua::lua_registerClassMethod(f_vm, "attach", ShaderAttachDrawable);
    Utils::Lua::lua_registerClassMethod(f_vm, "detach", ShaderDetachDrawable);
    LuaElementDef::AddHierarchyMethods(f_vm);
    Utils::Lua::lua_registerClassFinish(f_vm);
}

int ROC::LuaShaderDef::ShaderCreate(lua_State *f_vm)
{
    std::string l_vsp, l_fsp, l_gsp;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_vsp);
    argStream.ReadText(l_fsp);
    argStream.ReadNextText(l_gsp);
    if(!argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        Shader *l_shader = LuaManager::GetCore()->GetElementManager()->CreateShader(l_vsp, l_fsp, l_gsp);
        l_shader ? argStream.PushElement(l_shader) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

int ROC::LuaShaderDef::ShaderSetUniformValue(lua_State *f_vm)
{
    Shader *l_shader;
    std::string l_uniform;
    std::string l_type;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadText(l_uniform);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && !l_uniform.empty() && !l_type.empty())
    {
        switch(Utils::Enum::ReadEnumVector(g_UniformTypesTable, l_type))
        {
            case ROC_SHADER_UNIFORM_UINT:
            {
                unsigned int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_UVEC2:
            {
                glm::uvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_UVEC3:
            {
                glm::uvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_UVEC4:
            {
                glm::uvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_INT:
            {
                int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_IVEC2:
            {
                glm::ivec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_IVEC3:
            {
                glm::ivec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_IVEC4:
            {
                glm::ivec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_FLOAT:
            {
                float l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_VEC2:
            {
                glm::vec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_VEC3:
            {
                glm::vec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_VEC4:
            {
                glm::vec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_DOUBLE:
            {
                double l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValue(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_DVEC2:
            {
                glm::dvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_DVEC3:
            {
                glm::dvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_DVEC4:
            {
                glm::dvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_val);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_MAT2:
            {
                glm::mat2 l_mat;
                argStream.ReadVector(glm::value_ptr(l_mat), 4);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_MAT3:
            {
                glm::mat3 l_mat;
                argStream.ReadVector(glm::value_ptr(l_mat), 9);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            case ROC_SHADER_UNIFORM_MAT4:
            {
                glm::mat4 l_mat;
                argStream.ReadVector(glm::value_ptr(l_mat), 16);
                if(!argStream.HasErrors())
                {
                    LuaManager::GetCore()->GetRenderManager()->SetShaderUniformValueRef(l_shader, l_uniform, l_mat);
                    argStream.PushBoolean(true);
                }
                else argStream.PushBoolean(false);
            } break;
            default:
                argStream.PushBoolean(false);
        }
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaShaderDef::ShaderAttachDrawable(lua_State *f_vm)
{
    Shader *l_shader;
    Drawable *l_drawable;
    std::string l_uniform;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    argStream.ReadText(l_uniform);
    if(!argStream.HasErrors() && !l_uniform.empty())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->AttachDrawableToShader(l_shader, l_drawable, l_uniform);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int ROC::LuaShaderDef::ShaderDetachDrawable(lua_State *f_vm)
{
    Shader *l_shader;
    Drawable *l_drawable;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_shader);
    argStream.ReadElement(l_drawable);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::GetCore()->GetInheritManager()->DetachDrawableFromShader(l_shader, l_drawable);
        argStream.PushBoolean(l_result);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
