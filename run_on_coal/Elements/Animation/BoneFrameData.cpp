#include "stdafx.h"

#include "Elements/Animation/BoneFrameData.h"

namespace ROC
{

extern const glm::vec3 g_DefaultPosition;
extern const glm::quat g_DefaultRotation;
extern const glm::vec3 g_DefaultScale;
extern const float g_Epsilon;

}

ROC::BoneFrameData::BoneFrameData()
{
    std::memcpy(&m_position, &g_DefaultPosition, sizeof(glm::vec3));
    std::memcpy(&m_rotation, &g_DefaultRotation, sizeof(glm::quat));
    std::memcpy(&m_scale, &g_DefaultScale, sizeof(glm::vec3));
    m_useScale = false;
}
ROC::BoneFrameData::BoneFrameData(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec3 &f_scl)
{
    std::memcpy(&m_position, &f_pos, sizeof(glm::vec3));
    std::memcpy(&m_rotation, &f_rot, sizeof(glm::quat));
    std::memcpy(&m_scale, &f_scl, sizeof(glm::vec3));
    m_useScale = glm::all(glm::epsilonNotEqual(m_scale, g_DefaultScale, g_Epsilon));
}
ROC::BoneFrameData::~BoneFrameData()
{
}

bool ROC::BoneFrameData::IsEqual(BoneFrameData *f_data) const
{
    bool l_positionResult = glm::all(glm::epsilonEqual(m_position, f_data->m_position, g_Epsilon));
    bool l_rotationResult = glm::all(glm::epsilonEqual(m_rotation, f_data->m_rotation, g_Epsilon));
    bool l_scaleResult = glm::all(glm::epsilonEqual(m_scale, f_data->m_scale, g_Epsilon));
    return (l_positionResult && l_rotationResult && l_scaleResult);
}

void ROC::BoneFrameData::SetInterpolated(BoneFrameData *f_data, float f_blend)
{
    m_position = glm::lerp(m_position, f_data->m_position, f_blend);
    m_rotation = glm::slerp(m_rotation, f_data->m_rotation, f_blend);
    m_scale = glm::lerp(m_scale, f_data->m_scale, f_blend);
    m_useScale = (m_useScale || f_data->m_useScale);
}
void ROC::BoneFrameData::SetInterpolated(BoneFrameData *f_leftData, BoneFrameData *f_rightData, float f_blend)
{
    m_position = glm::lerp(f_leftData->m_position, f_rightData->m_position, f_blend);
    m_rotation = glm::slerp(f_leftData->m_rotation, f_rightData->m_rotation, f_blend);
    m_scale = glm::lerp(f_leftData->m_scale, f_rightData->m_scale, f_blend);
    m_useScale = (f_leftData->m_useScale || f_rightData->m_useScale);
}

bool ROC::BoneFrameData::Copy(BoneFrameData *f_src, BoneFrameData *f_dst)
{
    bool l_positionResult = glm::all(glm::epsilonEqual(f_src->m_position, f_dst->m_position, g_Epsilon));
    if(!l_positionResult) std::memcpy(&f_dst->m_position, &f_src->m_position, sizeof(glm::vec3));

    bool l_rotationResult = glm::all(glm::epsilonEqual(f_src->m_rotation, f_dst->m_rotation, g_Epsilon));
    if(!l_rotationResult) std::memcpy(&f_dst->m_rotation, &f_src->m_rotation, sizeof(glm::quat));

    bool l_scaleResult = glm::all(glm::epsilonEqual(f_src->m_scale, f_dst->m_scale, g_Epsilon));
    if(!l_scaleResult)
    {
        std::memcpy(&f_dst->m_scale, &f_src->m_scale, sizeof(glm::vec3));
        f_dst->m_useScale = f_src->m_useScale;
    }
    return (l_positionResult || l_rotationResult || l_scaleResult);
}
