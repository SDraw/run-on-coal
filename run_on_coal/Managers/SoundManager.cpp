#include "stdafx.h"
#include "Managers/SoundManager.h"

ROC::SoundManager::SoundManager(Core *f_core)
{
    m_core = f_core;

    m_listenerPosition = glm::vec3(0.f);
    m_listenerDirection = glm::vec3(0.f, 0.f, -1.f);
    m_listenerUp = glm::vec3(0.f, 1.f, 0.f);
    m_globalVolume = 100.f;
}
ROC::SoundManager::~SoundManager()
{
}

void ROC::SoundManager::SetListenerPosition(glm::vec3 &f_pos)
{
    if(std::memcmp(&f_pos, &m_listenerPosition, sizeof(glm::vec3)) != 0)
    {
        std::memcpy(&m_listenerPosition, &f_pos, sizeof(glm::vec3));
        sf::Listener::setPosition(m_listenerPosition.x, m_listenerPosition.y, m_listenerPosition.z);
    }
}

void ROC::SoundManager::SetListenerDirection(glm::vec3 &f_dir)
{
    if(std::memcmp(&f_dir, &m_listenerDirection, sizeof(glm::vec3)) != 0)
    {
        std::memcpy(&m_listenerDirection, &f_dir, sizeof(glm::vec3));
        sf::Listener::setDirection(m_listenerDirection.x, m_listenerDirection.y, m_listenerDirection.z);
    }
}

void ROC::SoundManager::SetListenerUp(glm::vec3 &f_up)
{
    if(std::memcmp(&f_up, &m_listenerUp, sizeof(glm::vec3)) != 0)
    {
        std::memcpy(&m_listenerUp, &f_up, sizeof(glm::vec3));
        sf::Listener::setUpVector(m_listenerUp.x, m_listenerUp.y, m_listenerUp.z);
    }
}

void ROC::SoundManager::SetGlobalVolume(float f_val)
{
    if(f_val != m_globalVolume)
    {
        m_globalVolume = f_val;
        btClamp(m_globalVolume, 0.f, 100.f);
        sf::Listener::setGlobalVolume(m_globalVolume);
    }
}