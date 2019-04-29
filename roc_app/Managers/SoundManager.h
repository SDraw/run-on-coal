#pragma once
#include "Interfaces/ISoundManager.h"

namespace ROC
{

class SoundManager final : public ISoundManager
{
    glm::vec3 m_listenerPosition;
    glm::vec3 m_listenerDirection;
    glm::vec3 m_listenerUp;
    float m_globalVolume;

    SoundManager(const SoundManager &that) = delete;
    SoundManager& operator=(const SoundManager &that) = delete;
public:
    void SetListenerPosition(const glm::vec3 &f_pos);
    const glm::vec3& GetListenerPosition() const;

    void SetListenerDirection(const glm::vec3 &f_dir);
    const glm::vec3& GetListenerDirection() const;

    void SetListenerUp(const glm::vec3 &f_up);
    const glm::vec3& GetListenerUp() const;

    void SetGlobalVolume(float f_val);
    float GetGlobalVolume() const;
protected:
    SoundManager();
    ~SoundManager();

    friend class Core;
};

}
