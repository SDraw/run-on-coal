#pragma once

namespace ROC
{
class Sound
{
    sf::Music* m_handle;

    float m_speed;
    float m_volume;
    bool m_b3D;
    bool m_looped;
    bool m_mono;

    glm::vec3 m_v3DPosition;
    glm::vec2 m_v3DDistance;
public:
    inline void Play() { m_handle->play(); }
    inline void Pause() { m_handle->pause(); }
    inline void Stop() { m_handle->stop(); }

    inline bool IsLooped() { return m_looped; }

    void SetSpeed(float f_speed);
    inline float GetSpeed() { return m_speed; }

    void SetVolume(float f_volume);
    inline float GetVolume() { return m_volume; }

    void SetTime(float f_time);
    float GetTime();
    float GetDuration();

    bool Set3DPositionEnabled(bool f_state);
    inline bool Get3DPositionEnabled() { return m_b3D; }

    bool Set3DPosition(glm::vec3 &f_pos);
    inline void Get3DPosition(glm::vec3 &f_pos) { std::memcpy(&f_pos, &m_v3DPosition, sizeof(glm::vec3)); }

    bool Set3DDistance(glm::vec2 &f_dist);
    inline void Get3DDistance(glm::vec2 &f_dist) { std::memcpy(&f_dist, &m_v3DDistance, sizeof(glm::vec2)); }

    inline int GetState() { return m_handle->getStatus(); }
protected:
    explicit Sound(bool f_loop);
    ~Sound();
    bool Load(std::string &f_path);
    friend class ElementManager;
};
}
