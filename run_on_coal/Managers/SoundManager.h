#pragma once

namespace ROC
{

class Core;
class SoundManager
{
    Core *m_core;

    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerDirection;
    glm::vec3 m_listenerUp;
    bool m_updatePosition;
    bool m_updateDirection;
    bool m_updateUp;
public:
    void SetListenerPosition(glm::vec3 &f_pos);
    void GetListenerPosition(glm::vec3 &f_pos);
    void SetListenerDirection(glm::vec3 &f_dir);
    void GetListenerDirection(glm::vec3 &f_dir);
    void SetListenerUp(glm::vec3 &f_up);
    void GetListenerUp(glm::vec3 &f_up);
protected:
    SoundManager(Core *f_core);
    ~SoundManager();
    void DoPulse();
    friend Core;
};

}
