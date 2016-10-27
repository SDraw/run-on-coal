#include "stdafx.h"
#include "Scene/Sound.h"

ROC::Sound::Sound(bool f_loop)
{
    m_handle = NULL;
    m_speed = 1.f;
    m_b3D = false;
    m_looped = f_loop;
    m_mono = false;
    m_v3DPosition = glm::vec3(0.f);
    m_v3DDistance = glm::vec2(0.f);
}
ROC::Sound::~Sound()
{
    if(m_handle) delete m_handle;
}

bool ROC::Sound::Load(std::string &f_path)
{
    if(m_handle) return false;
    m_handle = new sf::Music;
    if(!m_handle) return false;
    if(!m_handle->openFromFile(f_path)) return false;
    if(m_looped) m_handle->setLoop(true);
    m_mono = (m_handle->getChannelCount() == 1);
    return true;
}

void ROC::Sound::SetSpeed(float f_speed)
{
    m_speed = f_speed;
    m_handle->setPitch(m_speed);
}

void ROC::Sound::SetVolume(float f_volume)
{
    m_volume = f_volume;
    m_handle->setVolume(m_volume);
}

void ROC::Sound::SetTime(float f_time)
{
    sf::Time l_time = sf::seconds(f_time);
    m_handle->setPlayingOffset(l_time);
}
float ROC::Sound::GetTime()
{
    sf::Time l_time = m_handle->getPlayingOffset();
    return l_time.asSeconds();
}
float ROC::Sound::GetDuration()
{
    sf::Time l_duration = m_handle->getDuration();
    return l_duration.asSeconds();
}

bool ROC::Sound::Set3DPositionEnabled(bool f_state)
{
    if(!m_mono) return false;
    if(m_b3D == f_state) return true;
    m_b3D = f_state;
    if(m_b3D)
    {
        m_handle->setRelativeToListener(false);
        m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
        m_handle->setMinDistance(m_v3DDistance.x);
        m_handle->setAttenuation(m_v3DDistance.y);
    }
    else
    {
        m_handle->setRelativeToListener(true);
        m_v3DPosition = glm::vec3(0.f);
        m_v3DDistance = glm::vec2(0.f);
    }
    return true;
}

bool ROC::Sound::Set3DPosition(glm::vec3 &f_pos)
{
    if(!m_b3D) return false;
    std::memcpy(&m_v3DPosition, &f_pos, sizeof(glm::vec3));
    m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
    return true;
}

bool ROC::Sound::Set3DDistance(glm::vec2 &f_dist)
{
    if(!m_b3D) return false;
    std::memcpy(&m_v3DDistance, &f_dist, sizeof(glm::vec2));
    m_handle->setMinDistance(m_v3DDistance.x);
    m_handle->setAttenuation(m_v3DDistance.y);
    return true;
}