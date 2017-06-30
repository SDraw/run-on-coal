#include "stdafx.h"

#include "Utils/SystemTick.h"

unsigned int ROC::SystemTick::m_tick = 0U;

void ROC::SystemTick::UpdateTick()
{
    m_tick = GetTickCount();
}
