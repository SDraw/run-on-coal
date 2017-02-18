#pragma once

namespace ROC
{

class LuaArguments
{
    struct LuaArgument
    {
        union ArgumentValue
        {
            int m_int;
            double m_double;
            float m_float;
            void *m_pointer;
            std::string *m_string;
        } m_value;
        enum ArgumentType : unsigned char { Integer = 0U, Double, Float, Pointer, String } m_type;
        explicit LuaArgument(int f_val);
        explicit LuaArgument(double f_val);
        explicit LuaArgument(float f_val);
        explicit LuaArgument(void *f_val);
        explicit LuaArgument(std::string *f_val);
    };
    std::vector<LuaArgument> m_vArgs;
    int m_argCount;
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    template<typename T> void PushArgument(T f_val);
    void ProccessArguments(lua_State *f_vm);
    inline int GetArgumentsValue() const { return m_argCount; }
};

}

template<typename T> void ROC::LuaArguments::PushArgument(T f_val)
{
    m_vArgs.push_back(LuaArgument(f_val));
    m_argCount++;
};
