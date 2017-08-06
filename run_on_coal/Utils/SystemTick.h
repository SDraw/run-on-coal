#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int ms_tick;
    static unsigned int ms_delta;
public:
    inline static unsigned int GetTick() { return ms_tick; }
    inline static unsigned int GetDelta() { return ms_delta; }

    static void UpdateTick();
};

}
