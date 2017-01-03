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
    void ReadNumber(lua_Number &f_val);
    void ReadNumber(float &f_val);
    void ReadInteger(lua_Integer &f_val);
    void ReadInteger(int &f_val);
    void ReadInteger(unsigned int &f_val);
    void ReadText(std::string &f_val);
    void ReadUserdata(void **f_val, unsigned int f_type);
    void ReadPointer(void **f_val);
    void ReadFunction(int &f_val, void **f_pointer);
    void ReadFunctionPointer(void **f_pointer);

    void ReadNextBoolean(bool &f_val);
    void ReadNextNumber(lua_Number &f_val);
    void ReadNextNumber(float &f_val);
    void ReadNextInteger(lua_Integer &f_val);
    void ReadNextInteger(int &f_val);
    void ReadNextInteger(unsigned int &f_val);
    void ReadNextText(std::string &f_val);
    void ReadNextUserdata(void **f_val, unsigned int f_type);
    void ReadNextPointer(void **f_val);

    void ReadTableNumbers(std::vector<lua_Number> &f_vec, int f_size);
    void ReadTableNumbers(std::vector<float> &f_vec, int f_size);
    void ReadTableTexts(std::vector<std::string> &f_vec, int f_size);

    void PushBoolean(bool f_val);
    void PushNumber(lua_Number f_val);
    void PushInteger(lua_Integer f_val);
    void PushText(const std::string &f_val);
    void PushPointer(void *f_val);
    void PushMatrix(glm::mat4 &f_val);

    bool HasErrors();
    inline int GetReturnValue() { return m_returnValue; }
};
}
