#pragma once

namespace ROC
{

class BoneData
{
protected:
    std::string m_name;
    int m_parent;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    BoneData();
    ~BoneData();
    friend class Geometry;
    friend class Skeleton;
};

}
