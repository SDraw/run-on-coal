#pragma once

namespace ROC
{

class LuaArguments
{
    struct LuaArgument
    {
        union
        {
            int m_int;
            double m_double;
            float m_float;
            void *m_pointer;
        };
        std::string m_string;
        enum ArgumentType : unsigned char { Integer = 0U, Double, Float, Pointer, String } m_type;
    };
    LuaArgument m_dummyArgument;
    std::vector<LuaArgument> m_vArgs;
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    void PushArgument(int f_val);
    void PushArgument(double f_val);
    void PushArgument(float f_val);
    void PushArgument(void *f_val);
    void PushArgument(const std::string &f_val);
    void PushArgument(const char *f_fal, size_t f_size);
    void ProccessArguments(lua_State *f_vm);
    inline int GetArgumentsCount() const { return static_cast<int>(m_vArgs.size()); }
};

}
