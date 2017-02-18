#pragma once

namespace ROC
{

class SoundManager
{
    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerDirection;
    glm::vec3 m_listenerUp;
    float m_globalVolume;
public:
    void SetListenerPosition(glm::vec3 &f_pos);
    inline void GetListenerPosition(glm::vec3 &f_pos) { std::memcpy(&f_pos, &m_listenerPosition, sizeof(glm::vec3)); }
    void SetListenerDirection(glm::vec3 &f_dir);
    inline void GetListenerDirection(glm::vec3 &f_dir) { std::memcpy(&f_dir, &m_listenerDirection, sizeof(glm::vec3)); }
    void SetListenerUp(glm::vec3 &f_up);
    inline void GetListenerUp(glm::vec3 &f_up) { std::memcpy(&f_up, &m_listenerUp, sizeof(glm::vec3)); }

    void SetGlobalVolume(float f_val);
    inline float GetGlobalVolume() const { return m_globalVolume; }
protected:
    SoundManager();
    ~SoundManager();
    friend class Core;
};

}
