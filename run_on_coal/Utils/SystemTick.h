#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int m_tick;
public:
    inline static unsigned int GetTick() { return m_tick; }

    static void UpdateTick();
};

}
