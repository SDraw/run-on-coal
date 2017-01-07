#pragma once

namespace ROC
{

class BoneData
{
protected:
    std::string m_name;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    int m_parent;
    BoneData();
    ~BoneData();
    friend class Geometry;
    friend class Skeleton;
};

}
