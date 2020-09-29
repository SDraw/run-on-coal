#include "stdafx.h"

#include "Managers/SoundManager.h"

namespace ROC
{

extern const glm::vec3 g_emptyVec3;

}

ROC::SoundManager::SoundManager()
{
    m_listenerPosition = g_emptyVec3;
    m_listenerDirection = glm::vec3(0.f, 0.f, -1.f);
    m_listenerUp = glm::vec3(0.f, 1.f, 0.f);
    m_globalVolume = 100.f;
}

ROC::SoundManager::~SoundManager()
{
}

void ROC::SoundManager::SetListenerPosition(const glm::vec3 &f_pos)
{
    if(m_listenerPosition != f_pos)
    {
        std::memcpy(&m_listenerPosition, &f_pos, sizeof(glm::vec3));
        sf::Listener::setPosition(m_listenerPosition.x, m_listenerPosition.y, m_listenerPosition.z);
    }
}

const glm::vec3& ROC::SoundManager::GetListenerPosition() const
{
    return m_listenerPosition;
}

void ROC::SoundManager::SetListenerDirection(const glm::vec3 &f_dir)
{
    if(m_listenerDirection != f_dir)
    {
        std::memcpy(&m_listenerDirection, &f_dir, sizeof(glm::vec3));
        sf::Listener::setDirection(m_listenerDirection.x, m_listenerDirection.y, m_listenerDirection.z);
    }
}

const glm::vec3& ROC::SoundManager::GetListenerDirection() const
{
    return m_listenerDirection;
}

void ROC::SoundManager::SetListenerUp(const glm::vec3 &f_up)
{
    if(m_listenerUp != f_up)
    {
        std::memcpy(&m_listenerUp, &f_up, sizeof(glm::vec3));
        sf::Listener::setUpVector(m_listenerUp.x, m_listenerUp.y, m_listenerUp.z);
    }
}

const glm::vec3& ROC::SoundManager::GetListenerUp() const
{
    return m_listenerUp;
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

float ROC::SoundManager::GetGlobalVolume() const
{
    return m_globalVolume;
}
