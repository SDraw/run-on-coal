#pragma once

namespace ROC
{

class SystemTick
{
    static unsigned int s_tick;
public:
    inline static unsigned int GetTick() { return s_tick; }

    static void UpdateTick();
};

}
