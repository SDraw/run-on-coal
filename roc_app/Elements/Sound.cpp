#include "stdafx.h"

#include "Elements/Sound.h"

namespace ROC
{

extern const glm::vec2 g_EmptyVec2;
extern const glm::vec3 g_EmptyVec3;

}

ROC::Sound::Sound()
{
    m_elementType = ET_Sound;
    m_elementTypeName.assign("Sound");

    m_handle = nullptr;
    m_relative = false;
    m_looped = false;
    m_mono = false;
    m_v3DPosition = g_EmptyVec3;
    m_v3DDistance = g_EmptyVec2;
}
ROC::Sound::~Sound()
{
    delete m_handle;
}

bool ROC::Sound::Load(const std::string &f_path)
{
    if(!m_handle)
    {
        m_handle = new sf::Music;
        if(m_handle->openFromFile(f_path)) m_mono = (m_handle->getChannelCount() == 1);
        else
        {
            delete m_handle;
            m_handle = nullptr;
        }
    }
    return (m_handle != nullptr);
}

void ROC::Sound::Play()
{
    if(m_handle) m_handle->play();
}
void ROC::Sound::Pause()
{
    if(m_handle) m_handle->pause();
}
void ROC::Sound::Stop()
{
    if(m_handle) m_handle->stop();
}

bool  ROC::Sound::IsLooped() const
{
    return m_looped;
}
bool ROC::Sound::SetLoop(bool f_loop)
{
    if(m_handle)
    {
        if(m_looped != f_loop)
        {
            m_looped = f_loop;
            m_handle->setLoop(m_looped);
        }
    }
    return ((m_handle != nullptr) && (m_looped == f_loop));
}

float ROC::Sound::GetDuration()
{
    float l_duration = -1.f;
    if(m_handle) l_duration = m_handle->getDuration().asSeconds();
    return l_duration;
}

void ROC::Sound::SetSpeed(float f_speed)
{
    if(m_handle)
    {
        btClamp(f_speed, 0.f, std::numeric_limits<float>::max());
        m_handle->setPitch(f_speed);
    }
}
float ROC::Sound::GetSpeed()
{
    float l_speed = -1.f;
    if(m_handle) l_speed = m_handle->getPitch();
    return l_speed;
}

void ROC::Sound::SetVolume(float f_volume)
{
    if(m_handle)
    {
        btClamp(f_volume, 0.f, 100.f);
        m_handle->setVolume(f_volume);
    }
}
float ROC::Sound::GetVolume()
{
    float l_volume = -1.f;
    if(m_handle) l_volume = m_handle->getVolume();
    return l_volume;
}

void ROC::Sound::SetTime(float f_time)
{
    if(m_handle)
    {
        btClamp(f_time, 0.f, std::numeric_limits<float>::max());
        sf::Time l_time = sf::seconds(f_time);
        m_handle->setPlayingOffset(l_time);
    }
}
float ROC::Sound::GetTime()
{
    float l_time = -1.f;
    if(m_handle) l_time = m_handle->getPlayingOffset().asSeconds();
    return l_time;
}

bool ROC::Sound::Set3DPositionEnabled(bool f_state)
{
    if(m_handle)
    {
        if(m_mono)
        {
            if(m_relative != f_state)
            {
                m_relative = f_state;
                m_handle->setRelativeToListener(!m_relative);
                if(m_relative)
                {
                    m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
                    m_handle->setMinDistance(m_v3DDistance.x);
                    m_handle->setAttenuation(m_v3DDistance.y);
                }
                else
                {
                    std::memcpy(&m_v3DPosition, &g_EmptyVec3, sizeof(glm::vec3));
                    m_v3DDistance.x = m_v3DDistance.y = 0.f;
                }
            }
        }
    }
    return m_mono;
}
bool ROC::Sound::Get3DPositionEnabled() const
{
    return m_relative;
}

bool ROC::Sound::Set3DPosition(const glm::vec3 &f_pos)
{
    if(m_handle)
    {
        if(m_relative)
        {
            std::memcpy(&m_v3DPosition, &f_pos, sizeof(glm::vec3));
            m_handle->setPosition(m_v3DPosition.x, m_v3DPosition.y, m_v3DPosition.z);
        }
    }
    return m_relative;
}
const glm::vec3& ROC::Sound::Get3DPosition(glm::vec3 &f_pos) const
{
    return m_v3DPosition;
}

bool ROC::Sound::Set3DDistance(const glm::vec2 &f_dist)
{
    if(m_handle)
    {
        if(m_relative)
        {
            std::memcpy(&m_v3DDistance, &f_dist, sizeof(glm::vec2));
            m_handle->setMinDistance(m_v3DDistance.x);
            m_handle->setAttenuation(m_v3DDistance.y);
        }
    }
    return m_relative;
}
const glm::vec2& ROC::Sound::Get3DDistance(glm::vec2 &f_dist) const
{
    return m_v3DDistance;
}

int ROC::Sound::GetState() const
{
    int l_state = -1;
    if(m_handle) l_state = m_handle->getStatus();
    return l_state;
}
