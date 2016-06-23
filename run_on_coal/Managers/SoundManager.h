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
    bool m_update;
public:
    void SetListener3DOrientation(glm::vec3 &f_pos, glm::vec3 &f_dir, glm::vec3 &f_up);
    void GetListener3DOrientation(glm::vec3 &f_pos, glm::vec3 &f_dir, glm::vec3 &f_up);
protected:
    SoundManager(Core *f_core);
    ~SoundManager();
    void DoPulse();
    friend Core;
};

}
