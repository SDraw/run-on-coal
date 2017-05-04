#pragma once

namespace ROC
{

class LuaArguments;
class ArgReader
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
    void ReadFunction(int &f_val, void *&f_pointer);
    void ReadFunction(void *&f_pointer);

    void ReadNextBoolean(bool &f_val);
    template<typename T> void ReadNextNumber(T &f_val);
    template<typename T> void ReadNextInteger(T &f_val);
    void ReadNextText(std::string &f_val);
    template<class T> void ReadNextElement(T *&f_element);

    void ReadMatrix(float *f_val, int f_size);
    void ReadTableTexts(std::vector<std::string> &f_vec, int f_size);

    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushPointer(void *f_val);
    void PushMatrix(float *f_val, int f_size);

    void ReadArguments(LuaArguments &f_args);

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
                f_val = static_cast<T>(lua_tonumber(m_vm, m_currentArg));
                m_currentArg++;
                if(std::isnan(f_val) || std::isinf(f_val))
                {
                    m_error.assign("Got NaN/Inf");
                    m_hasErrors = true;
                }
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
            if(lua_isinteger(m_vm, m_currentArg))
            {
                f_val = static_cast<T>(lua_tointeger(m_vm, m_currentArg));
                m_currentArg++;
            }
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
            if(lua_islightuserdata(m_vm, m_currentArg))
            {
                Element *a = reinterpret_cast<Element*>(const_cast<void*>(lua_topointer(m_vm, m_currentArg)));
                if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(a))
                {
                    try
                    {
                        if((f_element = dynamic_cast<T*>(a)) != NULL) m_currentArg++;
                        else
                        {
                            m_error.assign("Wrong element type");
                            m_hasErrors = true;
                        }

                    }
                    catch(const std::exception&)
                    {
                        m_error.assign("Wrong element type");
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
        if(lua_isnumber(m_vm, m_currentArg))
        {
            f_val = static_cast<T>(lua_tonumber(m_vm, m_currentArg));
            m_currentArg++;
        }
    }
};
template<typename T> void ROC::ArgReader::ReadNextInteger(T &f_val)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_isinteger(m_vm, m_currentArg))
        {
            f_val = static_cast<T>(lua_tointeger(m_vm, m_currentArg));
            m_currentArg++;
        }
    }
};
template<class T> void ROC::ArgReader::ReadNextElement(T *&f_element)
{
    if(!m_hasErrors && (m_currentArg <= m_argCount))
    {
        if(lua_islightuserdata(m_vm, m_currentArg))
        {
            Element *a = reinterpret_cast<Element*>(const_cast<void*>(lua_topointer(m_vm, m_currentArg)));
            if(LuaManager::GetCore()->GetMemoryManager()->IsValidMemoryPointer(a))
            {
                try
                {
                    if((f_element = dynamic_cast<T*>(a)) != NULL) m_currentArg++;
                }
                catch(const std::exception&) {}
            }
        }
    }
}
