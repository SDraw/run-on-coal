#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/InheritanceManager.h"
#include "Managers/LuaManager.h"
#include "Managers/RenderManager.h"
#include "Scene/RenderTarget.h"
#include "Scene/Shader.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Shader.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_uniformTypesTable
{
    "uint", "uvec2", "uvec3", "uvec4",
    "int", "ivec2", "ivec3", "ivec4",
    "float", "vec2", "vec3", "vec4",
    "double", "dvec2", "dvec3", "dvec4",
    "mat2", "mat3", "mat4",
    "texture", "target"
};

int shaderCreate(lua_State *f_vm)
{
    std::string l_vsp, l_fsp, l_gsp;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadText(l_vsp);
    argStream.ReadText(l_fsp);
    argStream.ReadNextText(l_gsp);
    if(!argStream.HasErrors() && !l_vsp.empty() && !l_fsp.empty())
    {
        Shader *l_shader = LuaManager::m_corePointer->GetElementManager()->CreateShader(l_vsp, l_fsp, l_gsp);
        l_shader ? lua_pushlightuserdata(f_vm, l_shader) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int shaderDestroy(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_shader), ElementType::ShaderElement);
    if(!argStream.HasErrors())
    {
        bool l_result = LuaManager::m_corePointer->GetElementManager()->DestroyShader(l_shader);
        lua_pushboolean(f_vm, l_result);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int shaderGetUniform(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    std::string l_unif;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_shader), ElementType::ShaderElement);
    argStream.ReadText(l_unif);
    if(!argStream.HasErrors() && !l_unif.empty())
    {
        int l_result = l_shader->GetUniform(l_unif);
        (l_result != -1) ? lua_pushinteger(f_vm, l_result) : lua_pushboolean(f_vm, 0);
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}
int shaderSetUniformValue(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    int l_uniform;
    std::string l_type;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata(reinterpret_cast<void**>(&l_shader), ElementType::ShaderElement);
    argStream.ReadInteger(l_uniform);
    argStream.ReadText(l_type);
    if(!argStream.HasErrors() && (l_uniform >= 0 && l_uniform <= 255) && !l_type.empty())
    {
        switch(Utils::ReadEnumVector(g_uniformTypesTable, l_type))
        {
            case 0: // Unsigned int
            {
                unsigned int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 1: // Unsigned int 2
            {
                glm::uvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 2: // Unsigned int 3
            {
                glm::uvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 3: // Unsigned int 4
            {
                glm::uvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 4: // Integer
            {
                int l_val;
                argStream.ReadInteger(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 5: // Integer 2
            {
                glm::ivec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 6: // Integer 3
            {
                glm::ivec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 7: // Integer 4
            {
                glm::ivec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 8: // Float
            {
                float l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 9: // Float 2
            {
                glm::vec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 10: // Float 3
            {
                glm::vec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 11: // Float 4
            {
                glm::vec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            }
            case 12: // Double
            {
                double l_val;
                argStream.ReadNumber(l_val);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 13: // Double 2
            {
                glm::dvec2 l_val;
                for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 14: // Double 3
            {
                glm::dvec3 l_val;
                for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 15: // Double 4
            {
                glm::dvec4 l_val;
                for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
                if(!argStream.HasErrors())
                {
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_val);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            }
            case 16: // Matrix 2x2
            {
                std::vector<float> l_vec;
                argStream.DecreaseArguments(4);
                argStream.ReadTableNumbers(l_vec, 4);
                if(!argStream.HasErrors())
                {
                    glm::mat2 l_mat;
                    std::memcpy(&l_mat, l_vec.data(), sizeof(glm::mat2));
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_mat);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 17: // Matrix 3x3
            {
                std::vector<float> l_vec;
                argStream.DecreaseArguments(4);
                argStream.ReadTableNumbers(l_vec, 9);
                if(!argStream.HasErrors())
                {
                    glm::mat3 l_mat;
                    std::memcpy(&l_mat, l_vec.data(), sizeof(glm::mat3));
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_mat);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 18: // Matrix 4x4
            {
                std::vector<float> l_vec;
                argStream.DecreaseArguments(4);
                argStream.ReadTableNumbers(l_vec, 16);
                if(!argStream.HasErrors())
                {
                    glm::mat4 l_mat;
                    std::memcpy(&l_mat, l_vec.data(), sizeof(glm::mat4));
                    LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, l_uniform, l_mat);
                    lua_pushboolean(f_vm, 1);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 19: // Texture
            {
                Texture *l_texture;
                argStream.ReadUserdata(reinterpret_cast<void**>(&l_texture), ElementType::TextureElement);
                if(!argStream.HasErrors())
                {
                    bool l_result = LuaManager::m_corePointer->GetInheritManager()->AttachTextureToShader(l_shader, l_texture, l_uniform);
                    lua_pushboolean(f_vm, l_result);
                }
                else lua_pushboolean(f_vm, 0);
            } break;
            case 20: // Render target
            {
                RenderTarget *l_target;
                argStream.ReadUserdata(reinterpret_cast<void**>(&l_target), ElementType::RenderTargetElement);
                if(!argStream.HasErrors())
                {
                    bool l_result = LuaManager::m_corePointer->GetInheritManager()->AttachRenderTargetToShader(l_shader, l_target, l_uniform);
                    lua_pushboolean(f_vm, l_result);
                }
                else lua_pushboolean(f_vm, 1);
            } break;
            default:
                lua_pushboolean(f_vm, 0);
        }
    }
    else lua_pushboolean(f_vm, 0);
    return 1;
}

}
}