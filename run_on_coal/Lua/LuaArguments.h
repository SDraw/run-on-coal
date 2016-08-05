#pragma once

namespace ROC
{

class LuaArguments
{
    enum ArgumentType : char { Integer = 0U, Double, Float, Pointer, String };
    std::vector<std::pair<void*,ArgumentType>> m_vArgs;
    int m_argCount;
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    void PushArgument(int f_val);
    void PushArgument(double f_val);
    void PushArgument(float f_val);
    void PushArgument(void *f_val);
    void PushArgument(std::string &f_arg);
    void ProccessArguments(lua_State *f_vm);
    inline int GetArgumentsValue() { return m_argCount; }
};

}
