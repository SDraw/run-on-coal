#pragma once
namespace ROC
{

class BoneFrameData
{
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
protected:
    BoneFrameData();
    ~BoneFrameData();
    inline bool Compare(BoneFrameData *f_data)
    {
        return
            (
            std::memcmp(&m_position, &f_data->m_position, sizeof(glm::vec3)) ||
            std::memcmp(&m_rotation, &f_data->m_rotation, sizeof(glm::quat)) ||
            std::memcmp(&m_scale, &f_data->m_scale, sizeof(glm::vec3))
            );
    }
    inline void CopyTo(BoneFrameData *f_data)
    {
        std::memcpy(&f_data->m_position, &m_position, sizeof(glm::vec3));
        std::memcpy(&f_data->m_rotation, &m_rotation, sizeof(glm::quat));
        std::memcpy(&f_data->m_scale, &m_scale, sizeof(glm::vec3));
    }
    friend class Bone;
    friend class Skeleton;
};

}

