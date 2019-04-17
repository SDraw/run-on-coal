#pragma once

namespace ROC
{

class Animation;
class Bone;

class AnimationController final
{
    Animation *m_animation;
    unsigned int m_tick;
    enum AnimationControllerState
    { 
        ACS_None = 0U,
        ACS_Paused,
        ACS_Playing
    } m_state;
    float m_speed;

    bool m_blend;
    unsigned int m_blendTime;
    unsigned int m_blendTimeTick;
    float m_blendValue;

    AnimationController(const AnimationController &that) = delete;
    AnimationController& operator=(const AnimationController &that) = delete;
public:
    inline Animation* GetAnimation() const { return m_animation; };

    bool Play();
    bool Pause();
    bool Reset();

    inline bool IsPlaying() const { return (m_state == ACS_Playing); }
    inline bool IsPaused() const { return (m_state == ACS_Paused); }

    bool SetSpeed(float f_speed);
    inline float GetSpeed() const { return m_speed; }

    bool SetProgress(float f_val);
    float GetProgress() const;

    bool SetBlendTime(unsigned int f_val);
    inline unsigned int GetBlendTime() const { return m_blendTime; }
protected:
    AnimationController();
    ~AnimationController();

    void SetAnimation(Animation *f_anim);

    inline unsigned int GetTick() const { return m_tick; }
    inline float GetBlendValue() const { return m_blendValue; }

    void Update();

    friend class Model;
    friend class InheritanceManager;
};

}
