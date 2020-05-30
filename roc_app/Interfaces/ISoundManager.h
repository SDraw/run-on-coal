#pragma once

namespace ROC
{

class ISoundManager
{
public:
    virtual void SetListenerPosition(const glm::vec3 &f_pos) = 0;
    virtual const glm::vec3& GetListenerPosition() const = 0;
    virtual void SetListenerDirection(const glm::vec3 &f_dir) = 0;
    virtual const glm::vec3& GetListenerDirection() const = 0;
    virtual void SetListenerUp(const glm::vec3 &f_up) = 0;
    virtual const glm::vec3& GetListenerUp() const = 0;
    virtual void SetGlobalVolume(float f_val) = 0;
    virtual float GetGlobalVolume() const = 0;
};

}
