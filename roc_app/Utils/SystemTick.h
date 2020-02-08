#pragma once

namespace ROC
{

class SystemTick final
{
    static unsigned long long ms_tick;
    static unsigned long long ms_delta;
public:
    inline static unsigned long long GetTick() { return ms_tick; }
    inline static unsigned long long GetDelta() { return ms_delta; }

    static void Init();
    static void UpdateTick();
};

}
