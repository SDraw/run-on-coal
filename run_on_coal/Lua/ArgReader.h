#pragma once

namespace ROC
{
class ArgReader
{
    lua_State *m_vm;
    Core *m_core;
    int m_currentArg;
    int m_argCount;
    int m_returnValue;
    std::string m_error;
    bool m_hasErrors;
public:
    ArgReader(lua_State *f_vm, Core *f_core);
    ~ArgReader();

    void ReadBoolean(bool &f_val);
    template<typename T> void ReadNumber(T &f_val)
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
                        m_error.append("Got NaN/Inf");
                        m_hasErrors = true;
                    }
                }
                else
                {
                    m_error.append("Expected number");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.append("Not enough arguments");
                m_hasErrors = true;
            }
        }
    };
    template<typename T> void ReadInteger(T &f_val)
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
                    m_error.append("Expected integer");
                    m_hasErrors = true;
                }
            }
            else
            {
                m_error.append("Not enough arguments");
                m_hasErrors = true;
            }
        }
    };
    void ReadText(std::string &f_val);
    void ReadUserdata(void **f_val, unsigned char f_type);
    void ReadPointer(void **f_val);
    void ReadFunction(int &f_val, void **f_pointer);
    void ReadFunctionPointer(void **f_pointer);

    void ReadNextBoolean(bool &f_val);
    template<typename T> void ReadNextNumber(T &f_val)
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
    template<typename T> void ReadNextInteger(T &f_val)
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
    void ReadNextText(std::string &f_val);
    void ReadNextUserdata(void **f_val, unsigned char f_type);
    void ReadNextPointer(void **f_val);

    void ReadMatrix(float *f_val, int f_size);
    void ReadTableTexts(std::vector<std::string> &f_vec, int f_size);

    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushPointer(void *f_val);
    void PushMatrix(float *f_val, int f_size);

    bool HasErrors();
    inline int GetReturnValue() { return m_returnValue; }
};

}
