#pragma once
namespace ROC
{

class BoneFrameData
{
protected:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    BoneFrameData();
    ~BoneFrameData();

    // Returns true if data is different, false otherwise
    inline bool Compare(BoneFrameData *f_data)
    {
        return
            (
            (std::memcmp(&m_position, &f_data->m_position, sizeof(glm::vec3)) != 0) ||
            (std::memcmp(&m_rotation, &f_data->m_rotation, sizeof(glm::quat)) != 0) ||
            (std::memcmp(&m_scale, &f_data->m_scale, sizeof(glm::vec3)) != 0)
            );
    }
    inline void CopyFrom(BoneFrameData *f_data)
    {
        std::memcpy(&m_position, &f_data->m_position, sizeof(glm::vec3));
        std::memcpy(&m_rotation, &f_data->m_rotation, sizeof(glm::quat));
        std::memcpy(&m_scale, &f_data->m_scale, sizeof(glm::vec3));
    }
    friend class Animation;
    friend class Bone;
    friend class Skeleton;
};

}

