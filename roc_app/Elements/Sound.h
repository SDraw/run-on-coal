#pragma once
#include "Interfaces/ISound.h"
#include "Elements/Element.h"

namespace ROC
{

class Sound final : public Element, public virtual ISound
{
    sf::Music *m_handle;

    bool m_relative;
    bool m_looped;
    bool m_mono;

    glm::vec3 m_v3DPosition;
    glm::vec2 m_v3DDistance;

    Sound(const Sound &that) = delete;
    Sound& operator=(const Sound &that) = delete;
public:
    void Play();
    void Pause();
    void Stop();

    bool IsLooped() const;
    bool SetLoop(bool f_loop);

    float GetDuration();

    void SetSpeed(float f_speed);
    float GetSpeed();

    void SetVolume(float f_volume);
    float GetVolume();

    void SetTime(float f_time);
    float GetTime();

    bool Set3DPositionEnabled(bool f_state);
    bool Get3DPositionEnabled() const;

    bool Set3DPosition(const glm::vec3 &f_pos);
    const glm::vec3& Get3DPosition(glm::vec3 &f_pos) const;

    bool Set3DDistance(const glm::vec2 &f_dist);
    const glm::vec2& Get3DDistance(glm::vec2 &f_dist) const;

    int GetState() const;
protected:
    explicit Sound();
    ~Sound();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
