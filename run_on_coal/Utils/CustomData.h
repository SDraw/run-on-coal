#pragma once
#define CUSTOM_DATA_MAX_LENGTH 128U

namespace ROC
{

class CustomData
{
    union
    {
        bool m_bool;
        int m_int;
        double m_double;
        float m_float;
        void *m_pointer;
    };
    std::string m_string;
public:
    enum DataType : unsigned char { None = 0U, Boolean, Integer, Double, Float, Pointer, String };

    CustomData();
    ~CustomData();

    inline unsigned char GetType() { return m_type; }

    inline bool GetBoolean() { return m_bool; }
    void SetBoolean(bool f_val);

    inline int GetInteger() { return m_int; }
    void SetInteger(int f_val);

    inline double GetDouble() { return m_double; }
    void SetDouble(double f_val);

    inline float GetFloat() { return m_float; }
    void SetFloat(float f_val);

    inline void* GetPointer() { return m_pointer; }
    void SetPointer(void *f_val);

    inline void GetString(std::string &f_val) { f_val.assign(m_string); }
    void SetString(const std::string &f_val);
    void SetString(const char *f_val, size_t f_size);
    inline void ClearString() { m_string.clear(); }

    void operator=(CustomData &f_data);
private:
    DataType m_type;
protected:
    inline std::string& GetString() { return m_string; }

    friend class ArgReader;
    friend class LuaArguments;
};

}
