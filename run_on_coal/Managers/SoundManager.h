#pragma once

namespace ROC
{

class SoundManager final
{
    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerDirection;
    glm::vec3 m_listenerUp;
    float m_globalVolume;
public:
    void SetListenerPosition(const glm::vec3 &f_pos);
    inline const glm::vec3& GetListenerPosition() const { return m_listenerPosition; }

    void SetListenerDirection(const glm::vec3 &f_dir);
    inline const glm::vec3& GetListenerDirection() const { return m_listenerDirection; }

    void SetListenerUp(const glm::vec3 &f_up);
    inline const glm::vec3& GetListenerUp() const { return m_listenerUp; }

    void SetGlobalVolume(float f_val);
    inline float GetGlobalVolume() const { return m_globalVolume; }
protected:
    SoundManager();
    ~SoundManager();

    friend class Core;
};

}
