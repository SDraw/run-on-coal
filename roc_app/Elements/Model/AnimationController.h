#pragma once

namespace ROC
{

class Animation;
class Bone;

class AnimationController final
{
    enum AnimationState : unsigned char
    {
        AS_None = 0U,
        AS_Paused,
        AS_Playing
    };

    Animation *m_animation;
    unsigned int m_tick;
    AnimationState m_state;
    float m_speed;
    bool m_loop;

    bool m_blend;
    unsigned int m_blendTime;
    unsigned int m_blendTimeTick;
    float m_blendValue;

    AnimationController(const AnimationController &that) = delete;
    AnimationController& operator=(const AnimationController &that) = delete;
public:
    Animation* GetAnimation() const;

    bool Play(bool f_loop);
    bool Pause();
    bool Reset();

    bool IsPlaying() const;
    bool IsPaused() const;

    bool SetSpeed(float f_speed);
    float GetSpeed() const;

    bool SetProgress(float f_val);
    float GetProgress() const;

    bool SetBlendTime(unsigned int f_val);
    unsigned int GetBlendTime() const;
protected:
    AnimationController();
    ~AnimationController();

    void SetAnimation(Animation *f_anim);

    unsigned int GetTick() const;
    float GetBlendValue() const;

    void Update();

    friend class Model;
};

}
