#include "stdafx.h"

#include "Utils/SystemTick.h"

unsigned long long ROC::SystemTick::ms_tick = 0U;
unsigned long long ROC::SystemTick::ms_delta = 0U;

void ROC::SystemTick::Init()
{
    ms_tick = GetTickCount64();
    ms_delta = 0U;
}

unsigned long long ROC::SystemTick::GetTick()
{
    return ms_tick;
}

unsigned long long ROC::SystemTick::GetDelta()
{
    return ms_delta;
}

void ROC::SystemTick::UpdateTick()
{
    unsigned long long l_tick = GetTickCount64();
    ms_delta = l_tick - ms_tick;
    ms_tick = l_tick;
}
