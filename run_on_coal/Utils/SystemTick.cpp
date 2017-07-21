#include "stdafx.h"

#include "Utils/SystemTick.h"

unsigned int ROC::SystemTick::ms_tick = 0U;

void ROC::SystemTick::UpdateTick()
{
    ms_tick = GetTickCount();
}
