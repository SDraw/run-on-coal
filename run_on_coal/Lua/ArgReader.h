#pragma once

namespace ROC
{
class ArgReader
{
    lua_State *m_pVM;
    Core *m_core;
    bool m_hasErrors;
    int m_iArgIndex;
    int m_iArgNum;
    std::string m_error;
public:
    ArgReader(lua_State *f_vm, Core *f_core);
    ~ArgReader();

    void DecreaseArguments(int f_args);

    void ReadBoolean(bool &f_val);
    void ReadNumber(lua_Number &f_val);
    void ReadInteger(LUA_INTEGER &f_val);
    void ReadText(std::string &f_val);
    void ReadUserdata(void **f_val, unsigned int f_type);
    void ReadPointer(void **f_val);
    void ReadFunction(int &f_val, void **f_pointer);
    void ReadFunctionPointer(void **f_pointer);

    void ReadNextBoolean(bool &f_val);
    void ReadNextNumber(lua_Number &f_val);
    void ReadNextInteger(LUA_INTEGER &f_val);
    void ReadNextText(std::string &f_val);
    void ReadNextUserdata(void **f_val, unsigned int f_type);
    void ReadNextPointer(void **f_val);

    void ReadTableNumbers(std::vector<lua_Number> &f_vec, int f_size);
    void ReadTableTexts(std::vector<std::string> &f_vec, int f_size);

    bool HasErrors();
    inline int GetArgsValue() { return m_iArgNum; }
};
}
