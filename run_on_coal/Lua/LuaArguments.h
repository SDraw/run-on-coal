#pragma once

namespace ROC
{

class LuaArguments
{
    std::vector<std::pair<void*,unsigned char>> m_vArgs;
    enum ArgumentType
    {
        Integer = 0U, UnsignedChar, Double, Float, Pointer, String,
        VectorInteger, VectorUnsignedChar, VectorDouble, VectorFloat, VectorPointer, VectorString
    };
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    void PushArgument(int f_val);
    void PushArgument(std::vector<int> &f_value);
    void PushArgument(unsigned char f_val);
    void PushArgument(std::vector<unsigned char> &f_value);
    void PushArgument(double f_val);
    void PushArgument(std::vector<double> &f_value);
    void PushArgument(float f_val);
    void PushArgument(std::vector<float> &f_value);
    void PushArgument(void *f_val);
    void PushArgument(std::vector<void*> &f_value);
    void PushArgument(std::string &f_arg);
    void PushArgument(std::vector<std::string> &f_value);
    void ProccessArguments(lua_State *f_vm);
    size_t GetArgumentsValue();
};

}
