#pragma once

namespace ROC
{

class Element;
class CustomData;
class LuaArguments;
class LuaFunction;

class ArgReader final
{
    lua_State *m_vm;
    int m_argCurrent;
    int m_argCount;
    int m_returnCount;
    std::string m_error;
    bool m_hasErrors;

    ArgReader(const ArgReader &that);
    ArgReader& operator=(const ArgReader &that);

    void SetError(const char *f_str);
public:
    explicit ArgReader(lua_State *f_vm);
    ~ArgReader();

    void ReadBoolean(bool &f_val);
    template<typename T> void ReadNumber(T &f_val);
    template<typename T> void ReadInteger(T &f_val);
    void ReadText(std::string &f_val);
    void ReadFunction(LuaFunction &f_func);
    void ReadArguments(LuaArguments &f_args);
    template<class T> void ReadElement(T *&f_element);
    void ReadCustomData(CustomData &f_data);

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

    void PushNil();
    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushElement(Element *f_element);
    void PushCustomData(const CustomData &f_data);

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
                if(std::isnan(l_number) || std::isinf(l_number)) SetError("Got NaN/Inf");
                else f_val = static_cast<T>(l_number);
            }
            else SetError("Expected number");
        }
        else SetError("Not enough arguments");
    }
};
template<typename T> void ROC::ArgReader::ReadInteger(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_argCurrent)) f_val = static_cast<T>(lua_tointeger(m_vm, m_argCurrent++));
            else SetError("Expected integer");
        }
        else SetError("Not enough arguments");
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
                if(Core::GetCore()->GetElementManager()->IsValidElement(l_element))
                {
                    if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_argCurrent++;
                    else SetError("Invalid element");
                }
                else SetError("Invalid element");
            }
            else SetError("Expected element");
        }
        else SetError("Not enough arguments");
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
            if(Core::GetCore()->GetElementManager()->IsValidElement(l_element))
            {
                if((f_element = dynamic_cast<T*>(l_element)) != nullptr) m_argCurrent++;
            }
        }
    }
}
