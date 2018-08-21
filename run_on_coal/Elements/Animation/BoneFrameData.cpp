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
    m_position = g_DefaultPosition;
    m_rotation = g_DefaultRotation;
    m_scale = g_DefaultScale;
    m_useScale = false;
}
ROC::BoneFrameData::BoneFrameData(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec3 &f_scl)
{
    m_position = f_pos;
    m_rotation = f_rot;
    m_scale = f_scl;
    m_useScale = glm::all(glm::epsilonNotEqual(m_scale, g_DefaultScale, g_Epsilon));
}
ROC::BoneFrameData::~BoneFrameData()
{
}

bool ROC::BoneFrameData::IsEqual(const BoneFrameData *f_data) const
{
    bool l_positionResult = (m_position == f_data->m_position);
    bool l_rotationResult = (m_rotation == f_data->m_rotation);
    bool l_scaleResult = (m_scale == f_data->m_scale);
    return (l_positionResult && l_rotationResult && l_scaleResult);
}

void ROC::BoneFrameData::SetInterpolated(BoneFrameData *f_data, float f_blend)
{
    m_position = glm::mix(m_position, f_data->m_position, f_blend);
    m_rotation = glm::slerp(m_rotation, f_data->m_rotation, f_blend);
    m_scale = glm::mix(m_scale, f_data->m_scale, f_blend);
    m_useScale = (m_useScale || f_data->m_useScale);
}
void ROC::BoneFrameData::SetInterpolated(BoneFrameData *f_leftData, BoneFrameData *f_rightData, float f_blend)
{
    m_position = glm::mix(f_leftData->m_position, f_rightData->m_position, f_blend);
    m_rotation = glm::slerp(f_leftData->m_rotation, f_rightData->m_rotation, f_blend);
    m_scale = glm::mix(f_leftData->m_scale, f_rightData->m_scale, f_blend);
    m_useScale = (f_leftData->m_useScale || f_rightData->m_useScale);
}

bool ROC::BoneFrameData::Copy(const BoneFrameData *f_src, BoneFrameData *f_dst)
{
    bool l_positionResult = (f_src->m_position != f_dst->m_position);
    if(l_positionResult) std::memcpy(&f_dst->m_position, &f_src->m_position, sizeof(glm::vec3));

    bool l_rotationResult = (f_src->m_rotation != f_dst->m_rotation);
    if(l_rotationResult) std::memcpy(&f_dst->m_rotation, &f_src->m_rotation, sizeof(glm::quat));

    bool l_scaleResult = (f_src->m_scale != f_dst->m_scale);
    if(l_scaleResult)
    {
        std::memcpy(&f_dst->m_scale, &f_src->m_scale, sizeof(glm::vec3));
        f_dst->m_useScale = f_src->m_useScale;
    }
    return (l_positionResult || l_rotationResult || l_scaleResult);
}
