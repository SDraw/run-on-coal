#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned long m_tick;
public:
    static void GetTick(unsigned long &f_tick);
protected:
    static void UpdateTick();
    friend class Core;
};

}

