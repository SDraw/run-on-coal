#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int m_tick;
public:
    static unsigned int GetTick();
protected:
    static void UpdateTick();
    friend class Core;
};

}

