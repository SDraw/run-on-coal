#include "stdafx.h"
#include "Managers/SoundManager.h"

ROC::SoundManager::SoundManager(Core *f_core)
{
    m_core = f_core;

    m_listenerPosition = glm::vec3(0.f);
    m_listenerDirection = glm::vec3(0.f,0.f,-1.f);
    m_listenerUp = glm::vec3(0.f,1.f,0.f);
    m_updatePosition = true;
    m_updateDirection = true;
    m_updateUp = true;
}
ROC::SoundManager::~SoundManager()
{
}


void ROC::SoundManager::SetListenerPosition(glm::vec3 &f_pos)
{
    if(!std::memcmp(&f_pos,&m_listenerPosition,sizeof(glm::vec3))) return;
    std::memcpy(&m_listenerPosition,&f_pos,sizeof(glm::vec3));
    m_updatePosition = true;
}

void ROC::SoundManager::SetListenerDirection(glm::vec3 &f_dir)
{
    if(!std::memcmp(&f_dir,&m_listenerDirection,sizeof(glm::vec3))) return;
    std::memcpy(&m_listenerDirection,&f_dir,sizeof(glm::vec3));
    m_updateDirection = true;
}

void ROC::SoundManager::SetListenerUp(glm::vec3 &f_up)
{
    if(!std::memcmp(&f_up,&m_listenerUp,sizeof(glm::vec3))) return;
    std::memcpy(&m_listenerUp,&f_up,sizeof(glm::vec3));
    m_updateUp = true;
}

void ROC::SoundManager::DoPulse()
{
    if(m_updatePosition)
    {
        sf::Listener::setPosition(m_listenerPosition.x,m_listenerPosition.y,m_listenerPosition.z);
        m_updatePosition = false;
    }
    if(m_updateDirection)
    {
        sf::Listener::setDirection(m_listenerDirection.x,m_listenerDirection.y,m_listenerDirection.z);
        m_updateDirection = false;
    }
    if(m_updateUp)
    {
        sf::Listener::setUpVector(m_listenerUp.x,m_listenerUp.y,m_listenerUp.z);
        m_updateUp = false;
    }
}
