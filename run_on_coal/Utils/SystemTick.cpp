#include "stdafx.h"
#include "Utils/SystemTick.h"

unsigned int ROC::SystemTick::m_tick = 0U;

void ROC::SystemTick::UpdateTick()
{
#ifdef _WIN32
    m_tick = GetTickCount();
#elif __linux__
    struct timeval tv;
    if(!gettimeofday(&tv, NULL)) m_tick = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}
