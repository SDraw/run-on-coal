#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Sound final : public Element
{
    sf::Music *m_handle;

    bool m_relative;
    bool m_looped;
    bool m_mono;

    glm::vec3 m_v3DPosition;
    glm::vec2 m_v3DDistance;
public:
    inline bool IsLooped() const { return m_looped; }
    inline float GetDuration() const { return (m_handle ? m_handle->getDuration().asSeconds() : -1.f); }

    void Play();
    void Pause();
    void Stop();

    void SetSpeed(float f_speed);
    inline float GetSpeed() const { return (m_handle ? m_handle->getPitch() : -1.f); }

    void SetVolume(float f_volume);
    inline float GetVolume() const { return (m_handle ? m_handle->getVolume() : -1.f); }

    void SetTime(float f_time);
    inline float GetTime() const { return (m_handle ? m_handle->getPlayingOffset().asSeconds() : -1.f); }

    bool Set3DPositionEnabled(bool f_state);
    inline bool Get3DPositionEnabled() const { return m_relative; }

    bool Set3DPosition(const glm::vec3 &f_pos);
    inline const glm::vec3& Get3DPosition(glm::vec3 &f_pos) const { return m_v3DPosition; }

    bool Set3DDistance(const glm::vec2 &f_dist);
    inline const glm::vec2& Get3DDistance(glm::vec2 &f_dist) const { return m_v3DDistance; }

    inline int GetState() const { return (m_handle ? m_handle->getStatus() : -1); }
protected:
    explicit Sound(bool f_loop);
    ~Sound();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
