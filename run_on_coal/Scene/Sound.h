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

    bool Set3DPositionEnabled(bool f_state);
    bool Get3DPositionEnabled();

    bool Set3DPosition(glm::vec3 &f_pos);
    bool Get3DPosition(glm::vec3 &f_pos);

    bool Set3DDistance(glm::vec2 &f_dist);
    bool Get3DDistance(glm::vec2 &f_dist);

    inline int GetState() { return m_handle->getStatus(); }
protected:
    Sound(bool f_loop);
    ~Sound();
    bool Load(std::string &f_path);
    friend class ElementManager;
};

}
