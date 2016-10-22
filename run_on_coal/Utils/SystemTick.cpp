#include "stdafx.h"
#include "Utils/SystemTick.h"

unsigned long ROC::SystemTick::m_tick = GetTickCount();

void ROC::SystemTick::GetTick(unsigned long &f_tick)
{
    f_tick = m_tick;
}
void ROC::SystemTick::UpdateTick()
{
#ifdef _WIN32
    m_tick = GetTickCount();
#elif __linux__
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0) return 0;
    m_tick = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}