#include "stdafx.h"

#include "Utils/SystemTick.h"

unsigned int ROC::SystemTick::s_tick = 0U;

void ROC::SystemTick::UpdateTick()
{
    s_tick = GetTickCount();
}
