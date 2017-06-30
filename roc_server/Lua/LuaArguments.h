#pragma once
#include "Utils/CustomData.h"

namespace ROC
{

class LuaArguments final
{
    CustomData m_dummyData;
    std::vector<CustomData> m_vArgs;
public:
    LuaArguments();
    ~LuaArguments();

    void Clear();
    void PushArgument(bool f_val);
    void PushArgument(int f_val);
    void PushArgument(double f_val);
    void PushArgument(float f_val);
    void PushArgument(void *f_val, const std::string &f_name);
    void PushArgument(const std::string &f_val);
    void PushArgument(const char *f_fal, size_t f_size);

    inline int GetArgumentsCount() const { return static_cast<int>(m_vArgs.size()); }
    const std::vector<CustomData>& GetArgumentsVectorRef() const { return m_vArgs; }
};

}
