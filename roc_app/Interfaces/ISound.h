#pragma once
#include "IElement.h"

namespace ROC
{

class ISound : public virtual IElement
{
public:
    virtual void Play() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;

    virtual bool IsLooped() const = 0;
    virtual bool SetLoop(bool f_loop) = 0;

    virtual float GetDuration() = 0;

    virtual void SetSpeed(float f_speed) = 0;
    virtual float GetSpeed() = 0;

    virtual void SetVolume(float f_volume) = 0;
    virtual float GetVolume() = 0;

    virtual void SetTime(float f_time) = 0;
    virtual float GetTime() = 0;

    virtual bool Set3DPositionEnabled(bool f_state) = 0;
    virtual bool Get3DPositionEnabled() const = 0;

    virtual bool Set3DPosition(const glm::vec3 &f_pos) = 0;
    virtual const glm::vec3& Get3DPosition(glm::vec3 &f_pos) const = 0;

    virtual bool Set3DDistance(const glm::vec2 &f_dist) = 0;
    virtual const glm::vec2& Get3DDistance(glm::vec2 &f_dist) const = 0;

    virtual int GetState() const = 0;
};

}
