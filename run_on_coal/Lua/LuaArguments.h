#pragma once

namespace ROC
{

class LuaArguments
{
    enum ArgumentType
    {
        Integer = 0U, UnsignedChar, Double, Float, Pointer, String,
        VectorUnsignedChar, VectorFloat
    };
    std::vector<std::pair<void*,ArgumentType>> m_vArgs;
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    void PushArgument(int f_val);
    void PushArgument(unsigned char f_val);
    void PushArgument(const unsigned char *f_pointer, size_t f_count);
    void PushArgument(double f_val);
    void PushArgument(float f_val);
    void PushArgument(const float *f_pointer, size_t f_count);
    void PushArgument(void *f_val);
    void PushArgument(std::string &f_arg);
    void ProccessArguments(lua_State *f_vm);
    size_t GetArgumentsValue();
};

}
