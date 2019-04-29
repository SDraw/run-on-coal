#include "stdafx.h"

#include "Elements/Model/AnimationController.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Model/Bone.h"

#include "Utils/SystemTick.h"

ROC::AnimationController::AnimationController()
{
    m_animation = nullptr;
    m_tick = 0U;
    m_state = ACS_None;
    m_speed = 1.f;
    m_blend = true;
    m_blendTime = 500U;
    m_blendTimeTick = 0U;
    m_blendValue = 1.f;
}
ROC::AnimationController::~AnimationController()
{
}

void ROC::AnimationController::SetAnimation(Animation *f_anim)
{
    m_animation = f_anim;
    if(m_animation)
    {
        m_tick = 0U;
        m_blend = true;
        m_blendTimeTick = 0U;
        if(m_state == ACS_None) m_state = ACS_Paused;
    }
    else m_state = ACS_None;
}

bool ROC::AnimationController::Play()
{
    if(m_animation)
    {
        if(m_state != ACS_Playing) m_state = ACS_Playing;
    }
    return (m_animation != nullptr);
}
bool ROC::AnimationController::Pause()
{
    if(m_animation) m_state = ACS_Paused;
    return (m_animation != nullptr);
}
bool ROC::AnimationController::Reset()
{
    if(m_animation) m_tick = 0U;
    return (m_animation != nullptr);
}

bool ROC::AnimationController::SetSpeed(float f_speed)
{
    if(m_animation)
    {
        m_speed = f_speed;
        btClamp(m_speed, 0.f, std::numeric_limits<float>::max());
    }
    return (m_animation != nullptr);
}

bool ROC::AnimationController::SetProgress(float f_val)
{
    if(m_animation)
    {
        btClamp(f_val, 0.f, 1.f);
        m_tick = static_cast<unsigned int>(static_cast<float>(m_animation->GetDuration())*f_val);
    }
    return (m_animation != nullptr);
}
float ROC::AnimationController::GetProgress() const
{
    float l_result = 0.f;
    if(m_animation) l_result = (static_cast<float>(m_tick) / static_cast<float>(m_animation->GetDuration()));
    return l_result;
}

bool ROC::AnimationController::SetBlendTime(unsigned int f_val)
{
    if(m_animation)
    {
        m_blendTime = f_val;
        btClamp(m_blendTime, 1U, std::numeric_limits<unsigned int>::max());
    }
    return (m_animation != nullptr);
}

void ROC::AnimationController::Update()
{
    if(m_animation && (m_state == ACS_Playing))
    {
        m_tick += static_cast<unsigned int>(static_cast<float>(SystemTick::GetDelta())*m_speed);
        m_tick %= m_animation->GetDuration();
        if(m_blend)
        {
            m_blendTimeTick += static_cast<unsigned int>(SystemTick::GetDelta());
            if(m_blendTimeTick >= m_blendTime)
            {
                m_blendTimeTick = m_blendTime;
                m_blend = false;
                m_blendValue = 1.f;
            }
            else m_blendValue = static_cast<float>(m_blendTimeTick) / static_cast<float>(m_blendTime);
        }
    }
}
