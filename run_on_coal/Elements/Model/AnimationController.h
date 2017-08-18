#pragma once

namespace ROC
{

class Animation;
class Bone;
class AnimationController final
{
    Animation *m_animation;
    unsigned int m_tick;
    enum AnimationState { None = 0U, Paused, Playing } m_state;
    float m_speed;

    bool m_blend;
    unsigned int m_blendTime;
    unsigned int m_blendTimeTick;
public:
    inline Animation* GetAnimation() { return m_animation; };

    bool Play();
    bool Pause();
    bool Reset();

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

    void Update(std::vector<Bone*> &f_bones);

    friend class Model;
    friend class InheritanceManager;
};

}
