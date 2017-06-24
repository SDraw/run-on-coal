#pragma once

namespace ROC
{

struct LuaFunction
{
    void *m_ptr = nullptr;
    int m_ref = 0;
    bool m_removeRef = false;
};

}
