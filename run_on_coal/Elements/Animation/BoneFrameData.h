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

    bool IsEqual(BoneFrameData *f_data) const;

    void SetInterpolated(BoneFrameData *f_data, float f_blend);
    void SetInterpolated(BoneFrameData *f_leftData, BoneFrameData *f_rightData, float f_blend);

    static void Copy(BoneFrameData *f_src, BoneFrameData *f_dst);
};

}

