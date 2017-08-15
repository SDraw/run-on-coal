#pragma once

namespace ROC
{

class Element;
class CustomData;
class LuaArguments;
struct LuaFunction;
typedef glm::quat Quat;

class ArgReader final
{
    lua_State *m_vm;
    int m_argCurrent;
    int m_argCount;
    int m_returnCount;
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
    void ReadFunction(LuaFunction &f_func, bool f_ref = false);
    void ReadArguments(LuaArguments &f_args);
    template<class T> void ReadElement(T *&f_element);
    void ReadCustomData(CustomData &f_data);
    void ReadQuat(Quat *&f_quat);

    bool IsNextBoolean();
    bool IsNextNumber();
    bool IsNextInteger();
    bool IsNextText();
    bool IsNextFunction();
    bool IsNextUserdata();

    void ReadNextBoolean(bool &f_val);
    template<typename T> void ReadNextNumber(T &f_val);
    template<typename T> void ReadNextInteger(T &f_val);
    void ReadNextText(std::string &f_val);
    template<class T> void ReadNextElement(T *&f_element);

    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushElement(Element *f_element);
    void PushElement(void *f_ptr, const std::string &f_name);
    void PushCustomData(const CustomData &f_data);
    void PushQuat(const Quat &f_quat);

    void RemoveReference(const LuaFunction &f_func);

    bool HasErrors();
    inline int GetReturnValue() const { return m_returnCount; }
};

}

template<typename T> void ROC::ArgReader::ReadNumber(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isnumber(m_vm, m_argCurrent))
            {
                lua_Number l_number = lua_tonumber(m_vm, m_argCurrent++);
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
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_argCurrent)) f_val = static_cast<T>(lua_tointeger(m_vm, m_argCurrent++));
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
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isuserdata(m_vm, m_argCurrent))
            {
                Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_argCurrent));
                if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(l_element))
                {
                    if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_argCurrent++;
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
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isnumber(m_vm, m_argCurrent)) f_val = static_cast<T>(lua_tonumber(m_vm, m_argCurrent++));
    }
};
template<typename T> void ROC::ArgReader::ReadNextInteger(T &f_val)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_argCurrent)) f_val = static_cast<T>(lua_tointeger(m_vm, m_argCurrent++));
    }
};
template<class T> void ROC::ArgReader::ReadNextElement(T *&f_element)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isuserdata(m_vm, m_argCurrent))
        {
            Element *l_element = *reinterpret_cast<Element**>(lua_touserdata(m_vm, m_argCurrent));
            if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(l_element))
            {
                if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_argCurrent++;
            }
        }
    }
}
