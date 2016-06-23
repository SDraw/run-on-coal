#include "stdafx.h"
#include "LuaArguments.h"

ROC::LuaArguments::LuaArguments()
{
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
            case ArgumentType::VectorInteger:
                delete (std::vector<int>*)iter.first;
                break;
            case ArgumentType::VectorUnsignedChar:
                delete (std::vector<unsigned char>*)iter.first;
                break;
            case ArgumentType::VectorDouble:
                delete (std::vector<double>*)iter.first;
                break;
            case ArgumentType::VectorFloat:
                delete (std::vector<float>*)iter.first;
                break;
            case ArgumentType::VectorPointer:
                delete (std::vector<void*>*)iter.first;
                break;
            case ArgumentType::VectorString:
                delete (std::vector<std::string>*)iter.first;
                break;
        }
    }
    m_vArgs.clear();
}

void ROC::LuaArguments::PushArgument(int f_val)
{
    int *l_temp = new int(f_val);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_temp,ArgumentType::Integer));
}
void ROC::LuaArguments::PushArgument(std::vector<int> &f_value)
{
    std::vector<int> *l_vector = new std::vector<int>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorInteger));
}

void ROC::LuaArguments::PushArgument(unsigned char f_val)
{
    unsigned char *l_temp = new unsigned char(f_val);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_temp,ArgumentType::UnsignedChar));
}
void ROC::LuaArguments::PushArgument(std::vector<unsigned char> &f_value)
{
    std::vector<unsigned char> *l_vector = new std::vector<unsigned char>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorUnsignedChar));
}

void ROC::LuaArguments::PushArgument(double f_val)
{
    double *l_temp = new double(f_val);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_temp,ArgumentType::Double));
}
void ROC::LuaArguments::PushArgument(std::vector<double> &f_value)
{
    std::vector<double> *l_vector = new std::vector<double>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorDouble));
}

void ROC::LuaArguments::PushArgument(float f_val)
{
    float *l_temp = new float(f_val);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_temp,ArgumentType::Float));
}
void ROC::LuaArguments::PushArgument(std::vector<float> &f_value)
{
    std::vector<float> *l_vector = new std::vector<float>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorFloat));
}

void ROC::LuaArguments::PushArgument(void *f_val)
{
    m_vArgs.push_back(std::pair<void*,unsigned char>(f_val,ArgumentType::Pointer));
}
void ROC::LuaArguments::PushArgument(std::vector<void*> &f_value)
{
    std::vector<void*> *l_vector = new std::vector<void*>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorPointer));
}

void ROC::LuaArguments::PushArgument(std::string &f_arg)
{
    std::string *l_temp = new std::string(f_arg);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_temp,ArgumentType::String));
}
void ROC::LuaArguments::PushArgument(std::vector<std::string> &f_value)
{
    std::vector<std::string> *l_vector = new std::vector<std::string>(f_value);
    m_vArgs.push_back(std::pair<void*,unsigned char>(l_vector,ArgumentType::VectorString));
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
            case ArgumentType::VectorInteger:
            {
                std::vector<int> *l_vector = (std::vector<int>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushinteger(f_vm,l_vector->at(i));
                    lua_settable(f_vm,-3);
                }
            } break;
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
            case ArgumentType::VectorDouble:
            {
                std::vector<double> *l_vector = (std::vector<double>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushnumber(f_vm,l_vector->at(i));
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
            case ArgumentType::VectorPointer:
            {
                std::vector<void*> *l_vector = (std::vector<void*>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushlightuserdata(f_vm,l_vector->at(i));
                    lua_settable(f_vm,-3);
                }
            } break;
            case ArgumentType::VectorString:
            {
                std::vector<std::string> *l_vector = (std::vector<std::string>*)iter.first;
                lua_newtable(f_vm);
                for(size_t i=0, j=l_vector->size(); i < j; i++)
                {
                    lua_pushnumber(f_vm,i+1);
                    lua_pushstring(f_vm,l_vector->at(i).c_str());
                    lua_settable(f_vm,-3);
                }
            } break;
        }
    }
}

size_t ROC::LuaArguments::GetArgumentsValue()
{
    return m_vArgs.size();
}
