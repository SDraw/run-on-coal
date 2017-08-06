#include "stdafx.h"

#include "Utils/SystemTick.h"

unsigned int ROC::SystemTick::ms_tick = 0U;
unsigned int ROC::SystemTick::ms_delta = 0U;

void ROC::SystemTick::UpdateTick()
{
    unsigned int l_tick = GetTickCount();
    ms_delta = l_tick - ms_tick;
    ms_tick = l_tick;
}
