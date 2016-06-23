#include "stdafx.h"
#include "Managers/SoundManager.h"

ROC::SoundManager::SoundManager(Core *f_core)
{
    m_core = f_core;

    m_listenerPosition = glm::vec3(0.f);
    m_listenerDirection = glm::vec3(0.f,0.f,-1.f);
    m_listenerUp = glm::vec3(0.f,1.f,0.f);
    m_update = true;
}
ROC::SoundManager::~SoundManager()
{
}

void ROC::SoundManager::SetListener3DOrientation(glm::vec3 &f_pos, glm::vec3 &f_dir, glm::vec3 &f_up)
{
    std::memcpy(&m_listenerDirection,&f_dir,sizeof(glm::vec3));
    std::memcpy(&m_listenerPosition,&f_pos,sizeof(glm::vec3));
    std::memcpy(&m_listenerUp,&f_up,sizeof(glm::vec3));
    m_update = true;
}
void ROC::SoundManager::GetListener3DOrientation(glm::vec3 &f_pos, glm::vec3 &f_dir, glm::vec3 &f_up)
{
    std::memcpy(&f_dir,&m_listenerDirection,sizeof(glm::vec3));
    std::memcpy(&f_pos,&m_listenerPosition,sizeof(glm::vec3));
    std::memcpy(&f_up,&m_listenerUp,sizeof(glm::vec3));
}

void ROC::SoundManager::DoPulse()
{
    if(!m_update) return;
    sf::Listener::setPosition(m_listenerPosition.x,m_listenerPosition.y,m_listenerPosition.z);
    sf::Listener::setDirection(m_listenerDirection.x,m_listenerDirection.y,m_listenerDirection.z);
    sf::Listener::setUpVector(m_listenerUp.x,m_listenerUp.y,m_listenerUp.z);
    m_update = false;
}
