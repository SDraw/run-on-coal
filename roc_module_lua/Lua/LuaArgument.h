#pragma once

class LuaArgument
{
    unsigned char m_type;

    union
    {
        bool m_bool;
        lua_Number m_number;
        lua_Integer m_integer;
        void *m_userdata;
    };
    std::string m_string;
public:
    enum LuaArgumentType : unsigned char
    {
        LAT_Nil = 0U,
        LAT_Boolean,
        LAT_Number,
        LAT_Integer,
        LAT_String,
        LAT_LightUserdata,
        LAT_Userdata
    };

    LuaArgument(); // Nil
    explicit LuaArgument(bool f_value); // Boolean
    explicit LuaArgument(lua_Number f_value); // Number
    explicit LuaArgument(lua_Integer f_value); // Integer
    explicit LuaArgument(const std::string &f_value); // String
    explicit LuaArgument(void *f_value); // Light userdata
    LuaArgument(void *f_value, const std::string &f_table); // Userdata
    ~LuaArgument();

    inline unsigned char GetType() const { return m_type; }
    inline bool GetBoolean() const { return m_bool; }
    inline lua_Number GetNumber() const { return m_number; }
    inline lua_Integer GetInteger() const { return m_integer; }
    inline const std::string& GetString() const { return m_string; }
    inline void* GetLightUserdata() const { return m_userdata; }
};
