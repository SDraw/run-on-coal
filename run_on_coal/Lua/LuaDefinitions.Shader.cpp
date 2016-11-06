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
std::vector<std::string> g_uniformTypesTable
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
    if(argStream.HasErrors() || (l_vsp.empty() && l_fsp.empty()))
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    Shader *l_shader = LuaManager::m_corePointer->GetElementManager()->CreateShader(l_vsp, l_fsp, l_gsp);
    l_shader ? lua_pushlightuserdata(f_vm, l_shader) : lua_pushboolean(f_vm, 0);
    return 1;
}
int shaderDestroy(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_shader, ElementType::ShaderElement);
    if(argStream.HasErrors())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    bool result = LuaManager::m_corePointer->GetElementManager()->DestroyShader(l_shader);
    lua_pushboolean(f_vm, result);
    return 1;
}
int shaderGetUniform(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    std::string l_unif;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_shader, ElementType::ShaderElement);
    argStream.ReadText(l_unif);
    if(argStream.HasErrors() || l_unif.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    lua_Integer l_result = l_shader->GetUniform(l_unif);
    (l_result == -1) ? lua_pushboolean(f_vm, 0) : lua_pushinteger(f_vm, l_result);
    return 1;
}
int shaderSetUniformValue(lua_State *f_vm)
{
    Shader *l_shader = NULL;
    lua_Integer l_unif;
    std::string l_type;
    ArgReader argStream(f_vm, LuaManager::m_corePointer);
    argStream.ReadUserdata((void**)&l_shader, ElementType::ShaderElement);
    argStream.ReadInteger(l_unif);
    argStream.ReadText(l_type);
    if(argStream.HasErrors() || l_unif < 0 || l_unif > 255 || l_type.empty())
    {
        lua_pushboolean(f_vm, 0);
        return 1;
    }
    switch(Utils::ReadEnumVector(g_uniformTypesTable, l_type))
    {
        // Unsigned int
        case 0:
        {
            lua_Integer l_val;
            argStream.ReadInteger(l_val);
            if(argStream.HasErrors() || l_val < 0)
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, static_cast<int>(l_unif), static_cast<unsigned int>(l_val));
        } break;
        case 1:
        {
            lua_Integer l_val[2];
            for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors() || l_val[0] < 0 || l_val[1] < 0)
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::uvec2 l_vec(l_val[0], l_val[1]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 2:
        {
            lua_Integer l_val[3];
            for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors() || l_val[0] < 0 || l_val[1] < 0 || l_val[2] < 0)
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::uvec3 l_vec(l_val[0], l_val[1], l_val[2]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 3:
        {
            lua_Integer l_val[4];
            for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors() || l_val[0] < 0 || l_val[1] < 0 || l_val[2] < 0 || l_val[3] < 0)
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::uvec4 l_vec(l_val[0], l_val[1], l_val[2], l_val[3]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        // Integer
        case 4:
        {
            lua_Integer l_val;
            argStream.ReadInteger(l_val);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, static_cast<int>(l_unif), static_cast<int>(l_val));
        } break;
        case 5:
        {
            lua_Integer l_val[2];
            for(int i = 0; i < 2; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::ivec2 l_vec(l_val[0], l_val[1]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 6:
        {
            lua_Integer l_val[3];
            for(int i = 0; i < 3; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::ivec3 l_vec(l_val[0], l_val[1], l_val[2]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 7:
        {
            lua_Integer l_val[4];
            for(int i = 0; i < 4; i++) argStream.ReadInteger(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::ivec4 l_vec(l_val[0], l_val[1], l_val[2], l_val[3]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        // Float
        case 8:
        {
            lua_Number l_val;
            argStream.ReadNumber(l_val);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, static_cast<int>(l_unif), static_cast<float>(l_val));
        } break;
        case 9:
        {
            lua_Number l_val[2];
            for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::vec2 l_vec(l_val[0], l_val[1]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 10:
        {
            lua_Number l_val[3];
            for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::vec3 l_vec(l_val[0], l_val[1], l_val[2]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 11:
        {
            lua_Number l_val[4];
            for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::vec4 l_vec(l_val[0], l_val[1], l_val[2], l_val[3]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        }
        // Double
        case 12:
        {
            lua_Number l_val;
            argStream.ReadNumber(l_val);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueO(l_shader, static_cast<int>(l_unif), l_val);
        } break;
        case 13:
        {
            lua_Number l_val[2];
            for(int i = 0; i < 2; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::dvec2 l_vec(l_val[0], l_val[1]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 14:
        {
            lua_Number l_val[3];
            for(int i = 0; i < 3; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::dvec3 l_vec(l_val[0], l_val[1], l_val[2]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        } break;
        case 15:
        {
            lua_Number l_val[4];
            for(int i = 0; i < 4; i++) argStream.ReadNumber(l_val[i]);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::dvec4 l_vec(l_val[0], l_val[1], l_val[2], l_val[3]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_vec);
        }
        // Matrix 2x2
        case 16:
        {
            argStream.DecreaseArguments(4);
            std::vector<lua_Number> l_dvec;
            argStream.ReadTableNumbers(l_dvec, 4);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::mat2 l_mat(l_dvec[0], l_dvec[1], l_dvec[2], l_dvec[3]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_mat);
        } break;
        // Matrix 3x3
        case 17:
        {
            argStream.DecreaseArguments(4);
            std::vector<lua_Number> l_dvec;
            argStream.ReadTableNumbers(l_dvec, 9);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::mat3 l_mat(l_dvec[0], l_dvec[1], l_dvec[2], l_dvec[3], l_dvec[4], l_dvec[5], l_dvec[6], l_dvec[7], l_dvec[8]);
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_mat);
        } break;
        //Matrix 4x4
        case 18:
        {
            argStream.DecreaseArguments(4);
            std::vector<lua_Number> l_dvec;
            argStream.ReadTableNumbers(l_dvec, 16);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            glm::mat4 l_mat(
                l_dvec[0], l_dvec[1], l_dvec[2], l_dvec[3],
                l_dvec[4], l_dvec[5], l_dvec[6], l_dvec[7],
                l_dvec[8], l_dvec[9], l_dvec[10], l_dvec[11],
                l_dvec[12], l_dvec[13], l_dvec[14], l_dvec[15]
                );
            LuaManager::m_corePointer->GetRenderManager()->SetShaderUniformValueM(l_shader, static_cast<int>(l_unif), l_mat);
        } break;
        //Texture
        case 19:
        {
            Texture *l_texture;
            argStream.ReadUserdata((void**)&l_texture, ElementType::TextureElement);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetInheritManager()->AttachTextureToShader(l_shader, l_texture, static_cast<int>(l_unif));
        } break;
        //Target
        case 20:
        {
            RenderTarget *l_target;
            argStream.ReadUserdata((void**)&l_target, ElementType::RenderTargetElement);
            if(argStream.HasErrors())
            {
                lua_pushboolean(f_vm, 0);
                return 1;
            }
            LuaManager::m_corePointer->GetInheritManager()->AttachRenderTargetToShader(l_shader, l_target, static_cast<int>(l_unif));
        }
    }
    lua_pushboolean(f_vm, 1);
    return 1;
}
}
}