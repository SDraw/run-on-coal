#pragma once

namespace ROC
{

class Element;
class CustomData;
class LuaArguments;
struct LuaFunction;
class ArgReader final
{
    lua_State *m_vm;
    int m_currentArg;
    int m_argCount;
    int m_returnValue;
    std::string m_error;
    bool m_hasErrors;

    ArgReader(const ArgReader& that);
    ArgReader &operator=(const ArgReader &that);
public:
    explicit ArgReader(lua_State *f_vm);
    ~ArgReader();

    void ReadBoolean(bool &f_val);
    template<typename T> void ReadNumber(T &f_val);
    template<typename T> void ReadInteger(T &f_val);
    void ReadText(std::string &f_val);
    template<class T> void ReadElement(T *&f_element);
    void ReadFunction(LuaFunction &f_func, bool f_ref = false);
    void ReadVector(float *f_val, int f_size);
    void ReadVector(std::vector<std::string> &f_vec, int f_size);
    void ReadCustomData(CustomData &f_data);
    void ReadArguments(LuaArguments &f_args);

    void ReadNextBoolean(bool &f_val);
    template<typename T> void ReadNextNumber(T &f_val);
    template<typename T> void ReadNextInteger(T &f_val);
    void ReadNextText(std::string &f_val);
    template<class T> void ReadNextElement(T *&f_element);

    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushVector(float *f_val, int f_size);
    void PushCustomData(CustomData &f_data);
    void PushElement(Element *f_element);
    void PushElement(void *f_ptr, const std::string &f_name);

    void RemoveReference(const LuaFunction &f_func);

    bool HasErrors();
    inline int GetReturnValue() const { return m_returnValue; }
};

}

template<typename T> void ROC::ArgReader::ReadNumber(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isnumber(m_vm, m_currentArg))
            {
                lua_Number l_number = lua_tonumber(m_vm, m_currentArg++);
                if(std::isnan(l_number) || std::isinf(l_number))
                {
                    m_error.assign("Got NaN/Inf");
                    m_hasErrors = true;
                }
                else f_val = static_cast<T>(l_number);
            }
            else
            {
                m_error.assign("Expected number");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
};
template<typename T> void ROC::ArgReader::ReadInteger(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_currentArg)) f_val = static_cast<T>(lua_tointeger(m_vm, m_currentArg++));
            else
            {
                m_error.assign("Expected integer");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
};
template<class T> void ROC::ArgReader::ReadElement(T *&f_element)
{
    if(!m_hasErrors)
    {
        if(m_currentArg <= m_argCount)
        {
            if(lua_isuserdata(m_vm, m_currentArg))
            {
                Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_currentArg));
                if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(l_element))
                {
                    if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_currentArg++;
                    else
                    {
                        m_error.assign("Invalid element");
                        m_hasErrors = true;
                    }
                }
                else
                {
                    m_error.assign("Invalid element");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.assign("Expected element");
                m_hasErrors = true;
            }
        }
        else
        {
            m_error.assign("Not enough arguments");
            m_hasErrors = true;
        }
    }
}

template<typename T> void ROC::ArgReader::ReadNextNumber(T &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isnumber(m_vm, m_currentArg)) f_val = static_cast<T>(lua_tonumber(m_vm, m_currentArg++));
    }
};
template<typename T> void ROC::ArgReader::ReadNextInteger(T &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_currentArg)) f_val = static_cast<T>(lua_tointeger(m_vm, m_currentArg++));
    }
};
template<class T> void ROC::ArgReader::ReadNextElement(T *&f_element)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isuserdata(m_vm, m_currentArg))
        {
            Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_currentArg));
            if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(l_element))
            {
                if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_currentArg++;
            }
        }
    }
}
