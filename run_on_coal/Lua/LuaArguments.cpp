#include "stdafx.h"
#include "LuaArguments.h"

ROC::LuaArguments::LuaArguments()
{
    m_argCount = 0U;
}
ROC::LuaArguments::~LuaArguments()
{
    Clear();
}

void ROC::LuaArguments::Clear()
{
    for(auto iter : m_vArgs)
    {
        switch(iter.second)
        {
            case ArgumentType::Integer:
                delete (int*)iter.first;
                break;
            case ArgumentType::Double:
                delete (double*)iter.first;
                break;
            case ArgumentType::String:
                delete (std::string*)iter.first;
                break;
            case ArgumentType::Float:
                delete (float*)iter.first;
                break;
            case ArgumentType::VectorUnsignedChar:
                delete (std::vector<unsigned char>*)iter.first;
                break;
            case ArgumentType::VectorFloat:
                delete (std::vector<float>*)iter.first;
                break;
        }
    }
    m_vArgs.clear();
    m_argCount = 0U;
}

void ROC::LuaArguments::PushArgument(int f_val)
{
    int *l_temp = new int(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Integer));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(unsigned char f_val)
{
    unsigned char *l_temp = new unsigned char(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::UnsignedChar));
    m_argCount++;
}
void ROC::LuaArguments::PushArgument(const unsigned char *f_pointer, size_t f_count)
{
    std::vector<unsigned char> *l_vector = new std::vector<unsigned char>{f_pointer,f_pointer+f_count};
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_vector,ArgumentType::VectorUnsignedChar));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(double f_val)
{
    double *l_temp = new double(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Double));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(float f_val)
{
    float *l_temp = new float(f_val);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::Float));
    m_argCount++;
}
void ROC::LuaArguments::PushArgument(const float *f_pointer, size_t f_count)
{
    std::vector<float> *l_vector = new std::vector<float>{f_pointer,f_pointer+f_count};
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_vector,ArgumentType::VectorFloat));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(void *f_val)
{
    m_vArgs.push_back(std::pair<void*,ArgumentType>(f_val,ArgumentType::Pointer));
    m_argCount++;
}

void ROC::LuaArguments::PushArgument(std::string &f_arg)
{
    std::string *l_temp = new std::string(f_arg);
    m_vArgs.push_back(std::pair<void*,ArgumentType>(l_temp,ArgumentType::String));
    m_argCount++;
}

void ROC::LuaArguments::ProccessArguments(lua_State *f_vm)
{
    for(auto iter : m_vArgs)
    {
        switch(iter.second)
        {
            case ArgumentType::Integer:
                lua_pushinteger(f_vm,*(int*)iter.first);
                break;
            case ArgumentType::UnsignedChar:
                lua_pushinteger(f_vm,*(unsigned char*)iter.first);
                break;
            case ArgumentType::Double:
                lua_pushnumber(f_vm,*(double*)iter.first);
                break;
            case ArgumentType::Float:
                lua_pushnumber(f_vm,*(float*)iter.first);
                break;
            case ArgumentType::Pointer:
                lua_pushlightuserdata(f_vm,iter.first);
                break;
            case ArgumentType::String:
                lua_pushstring(f_vm,((std::string*)iter.first)->c_str());
                break;
            case ArgumentType::VectorUnsignedChar:
            {
                std::vector<unsigned char> *l_vector = (std::vector<unsigned char>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushinteger(f_vm,l_vector->at(i));
                    lua_settable(f_vm,-3);
                }
            } break;
            case ArgumentType::VectorFloat:
            {
                std::vector<float> *l_vector = (std::vector<float>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushnumber(f_vm,l_vector->at(i));
                    lua_settable(f_vm,-3);
                }
            } break;
        }
    }
}

unsigned int ROC::LuaArguments::GetArgumentsValue()
{
    return m_argCount;
}
