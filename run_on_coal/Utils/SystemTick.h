#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int ms_tick;
public:
    inline static unsigned int GetTick() { return ms_tick; }

    static void UpdateTick();
};

}
