#pragma once

class LuaFunction;

typedef glm::quat Quat;

class ArgReader final
{
    lua_State *m_vm;
    int m_argCurrent;
    int m_argCount;
    int m_returnCount;
    std::string m_error;
    bool m_hasErrors;

    ArgReader(const ArgReader &that) = delete;
    ArgReader& operator=(const ArgReader &that) = delete;

    void SetError(const char *f_str);
public:
    explicit ArgReader(lua_State *f_vm);
    ~ArgReader();

    void ReadBoolean(bool &f_val);
    template<typename T> void ReadNumber(T &f_val);
    template<typename T> void ReadInteger(T &f_val);
    void ReadText(std::string &f_val);
    void ReadPointer(void *&f_val);
    void ReadFunction(LuaFunction &f_func);
    void ReadArguments(CustomArguments &f_args);
    template<class T> void ReadElement(T *&f_element);
    void ReadArgument(CustomArgument &f_data);
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
    void ReadNextPointer(void *&f_val);
    void ReadNextText(std::string &f_val);
    void ReadNextFunction(LuaFunction &f_func);
    template<class T> void ReadNextElement(T *&f_element);

    void PushNil();
    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushPointer(void *f_val);
    void PushText(const std::string &f_val);
    void PushElement(ROC::IElement *f_element);
    void PushArgument(const CustomArgument &f_data);
    void PushQuat(const Quat &f_quat);

    bool HasErrors();
    inline int GetReturnValue() const { return m_returnCount; }
};

template<typename T> void ArgReader::ReadNumber(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isnumber(m_vm, m_argCurrent))
            {
                lua_Number l_number = lua_tonumber(m_vm, m_argCurrent);
                if(!std::isnan(l_number) && !std::isinf(l_number))
                {
                    f_val = static_cast<T>(l_number);
                    m_argCurrent++;
                }
                else SetError("Got NaN/Inf");
            }
            else SetError("Expected number");
        }
        else SetError("Not enough arguments");
    }
};
template<typename T> void ArgReader::ReadInteger(T &f_val)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isinteger(m_vm, m_argCurrent))
            {
                f_val = static_cast<T>(lua_tointeger(m_vm, m_argCurrent));
                m_argCurrent++;
            }
            else SetError("Expected integer");
        }
        else SetError("Not enough arguments");
    }
};
template<class T> void ArgReader::ReadElement(T *&f_element)
{
    if(!m_hasErrors)
    {
        if(m_argCurrent <= m_argCount)
        {
            if(lua_isuserdata(m_vm, m_argCurrent))
            {
                ROC::IElement *l_element = *reinterpret_cast<ROC::IElement**>(lua_touserdata(m_vm, m_argCurrent));
                if(LuaModule::GetModule()->GetEngineCore()->GetElementManager()->IsValidElement(l_element))
                {
                    f_element = dynamic_cast<T*>(l_element);
                    if(f_element != nullptr) m_argCurrent++;
                    else SetError("Invalid element");
                }
                else SetError("Invalid element");
            }
            else SetError("Expected element");
        }
        else SetError("Not enough arguments");
    }
}

template<typename T> void ArgReader::ReadNextNumber(T &f_val)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isnumber(m_vm, m_argCurrent))
        {
            f_val = static_cast<T>(lua_tonumber(m_vm, m_argCurrent));
            m_argCurrent++;
        }
    }
};
template<typename T> void ArgReader::ReadNextInteger(T &f_val)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_argCurrent))
        {
            f_val = static_cast<T>(lua_tointeger(m_vm, m_argCurrent));
            m_argCurrent++;
        }
    }
};
template<class T> void ArgReader::ReadNextElement(T *&f_element)
{
    if(!m_hasErrors && (m_argCurrent <= m_argCount))
    {
        if(lua_isuserdata(m_vm, m_argCurrent))
        {
            ROC::IElement *l_element = *reinterpret_cast<ROC::IElement**>(lua_touserdata(m_vm, m_argCurrent));
            if(LuaModule::GetModule()->GetEngineCore()->GetElementManager()->IsValidElement(l_element))
            {
                f_element = dynamic_cast<T*>(l_element);
                if(f_element != nullptr) m_argCurrent++;
            }
        }
    }
}
