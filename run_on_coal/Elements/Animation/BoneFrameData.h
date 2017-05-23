#pragma once
namespace ROC
{

class BoneFrameData
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
public:
    BoneFrameData();
    BoneFrameData(const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec3 &f_scl);
    ~BoneFrameData();

    inline bool IsEqual(BoneFrameData *f_data) const
    {
        return ((m_position == f_data->m_position) && (m_rotation == f_data->m_rotation) && (m_scale == f_data->m_scale));
    }

    void SetInterpolated(BoneFrameData *f_data, float f_blend);
    void SetInterpolated(BoneFrameData *f_leftData, BoneFrameData *f_rightData, float f_blend);

    static void Copy(BoneFrameData *f_inputData, BoneFrameData *f_outputData);
protected:
    inline glm::vec3& GetPositionRef() { return m_position; }
    inline glm::quat& GetRotationRef() { return m_rotation; }
    inline glm::vec3& GetScaleRef() { return m_scale; }

    friend class Bone;
};

}

