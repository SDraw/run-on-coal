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
    void Play();
    void Pause();
    void Stop();

    inline bool IsLooped() const { return m_looped; }
    bool SetLoop(bool f_loop);

    float GetDuration();

    void SetSpeed(float f_speed);
    float GetSpeed();

    void SetVolume(float f_volume);
    float GetVolume();

    void SetTime(float f_time);
    float GetTime();

    bool Set3DPositionEnabled(bool f_state);
    inline bool Get3DPositionEnabled() const { return m_relative; }

    bool Set3DPosition(const glm::vec3 &f_pos);
    inline const glm::vec3& Get3DPosition(glm::vec3 &f_pos) const { return m_v3DPosition; }

    bool Set3DDistance(const glm::vec2 &f_dist);
    inline const glm::vec2& Get3DDistance(glm::vec2 &f_dist) const { return m_v3DDistance; }

    int GetState();
protected:
    explicit Sound();
    ~Sound();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
