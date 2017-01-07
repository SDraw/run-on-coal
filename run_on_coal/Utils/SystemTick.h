#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int m_tick;
    SystemTick() {}
public:
    inline static unsigned int GetTick() { return m_tick; }
protected:
    static void UpdateTick();
    friend class Core;
};

}
