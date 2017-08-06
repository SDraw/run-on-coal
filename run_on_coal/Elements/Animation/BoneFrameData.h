#pragma once
namespace ROC
{

class BoneFrameData
{
public:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    BoneFrameData();
    BoneFrameData(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec3 &f_scl);
    ~BoneFrameData();

    inline bool IsEqual(BoneFrameData *f_data) const
    {
        return ((m_position == f_data->m_position) && (m_rotation == f_data->m_rotation) && (m_scale == f_data->m_scale));
    }

    void SetInterpolated(BoneFrameData *f_data, float f_blend);
    void SetInterpolated(BoneFrameData *f_leftData, BoneFrameData *f_rightData, float f_blend);

    static void Copy(BoneFrameData *f_src, BoneFrameData *f_dst);
};

}

