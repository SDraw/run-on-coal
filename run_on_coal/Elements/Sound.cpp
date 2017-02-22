#include "stdafx.h"
#include "Elements/Sound.h"

ROC::Sound::Sound(bool f_loop)
{
    m_elementType = ElementType::SoundElement;

    m_handle = NULL;
    m_duration = 0.f;
    m_speed = 1.f;
    m_volume = 100.f;
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
    if(!m_handle)
    {
        m_handle = new sf::Music;
        if(m_handle->openFromFile(f_path))
        {
            if(m_looped) m_handle->setLoop(true);
            m_mono = (m_handle->getChannelCount() == 1);
            m_duration = m_handle->getDuration().asSeconds();
        }
        else
        {
            delete m_handle;
            m_handle = NULL;
        }
    }
    return (m_handle != NULL);
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
    return m_handle->getPlayingOffset().asSeconds();
}

bool ROC::Sound::Set3DPositionEnabled(bool f_state)
{
    if(m_mono)
    {
        if(m_b3D != f_state)
        {
            m_b3D = f_state;
            m_handle->setRelativeToListener(!m_b3D);
            if(m_b3D)
            {
                m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
                m_handle->setMinDistance(m_v3DDistance.x);
                m_handle->setAttenuation(m_v3DDistance.y);
            }
            else
            {
                m_v3DPosition.x = m_v3DPosition.y = m_v3DPosition.z = 0.f;
                m_v3DDistance.x = m_v3DDistance.y = 0.f;
            }
        }
    }
    return m_mono;
}

bool ROC::Sound::Set3DPosition(glm::vec3 &f_pos)
{
    if(m_b3D)
    {
        std::memcpy(&m_v3DPosition, &f_pos, sizeof(glm::vec3));
        m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
    }
    return m_b3D;
}

bool ROC::Sound::Set3DDistance(glm::vec2 &f_dist)
{
    if(m_b3D)
    {
        std::memcpy(&m_v3DDistance, &f_dist, sizeof(glm::vec2));
        m_handle->setMinDistance(m_v3DDistance.x);
        m_handle->setAttenuation(m_v3DDistance.y);
    }
    return m_b3D;
}
