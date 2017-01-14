#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/SfmlManager.h"

ROC::SfmlManager::SfmlManager(Core *f_core)
{
    m_core = f_core;
    m_time = 0.f;
}
ROC::SfmlManager::~SfmlManager()
{
}

void ROC::SfmlManager::DoPulse()
{
    m_time = m_clock.getElapsedTime().asSeconds();
}
