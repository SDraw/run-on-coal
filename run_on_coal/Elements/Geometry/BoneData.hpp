#pragma once

namespace ROC
{

struct BoneData
{
    std::string m_name;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    int m_parent = -1;
};

}
