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
        LuaArgument(int f_val) { m_value.m_int = f_val; m_type = Integer; }
        LuaArgument(double f_val) { m_value.m_double = f_val; m_type = Double; }
        LuaArgument(float f_val) { m_value.m_float = f_val; m_type = Float; }
        LuaArgument(void *f_val) { m_value.m_pointer = f_val; m_type = Pointer; }
        LuaArgument(std::string *f_val) { m_value.m_string = f_val; m_type = String; }
    };
    std::vector<LuaArgument> m_vArgs;
    int m_argCount;
public:
    LuaArguments();
    ~LuaArguments();
    void Clear();
    template<typename T> void PushArgument(T f_val)
    {
        m_vArgs.push_back(f_val);
        m_argCount++;
    };
    void ProccessArguments(lua_State *f_vm);
    inline int GetArgumentsValue() { return m_argCount; }
};

}
