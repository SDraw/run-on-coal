#pragma once
#include "Utils/CustomData.h"

namespace ROC
{

class Element;

class LuaArguments final
{
    CustomData m_dummyData;
    std::vector<CustomData> m_vArgs;

    LuaArguments(const LuaArguments &that);
    LuaArguments& operator=(const LuaArguments &that);
public:
    LuaArguments();
    ~LuaArguments();

    void Clear();
    void PushArgument(bool f_val);
    void PushArgument(int f_val);
    void PushArgument(double f_val);
    void PushArgument(float f_val);
    void PushArgument(Element *f_element);
    void PushArgument(const std::string &f_val);
    void PushArgument(const char *f_val, size_t f_size);

    inline int GetArgumentsCount() const { return static_cast<int>(m_vArgs.size()); }
    const std::vector<CustomData>& GetArgumentsVectorRef() const { return m_vArgs; }
};

}
