#pragma once

class Frame
{
protected:
    std::vector<glm::vec3> m_position;
    std::vector<glm::quat> m_rotation;
    std::vector<glm::vec3> m_scale;
    Frame();
    ~Frame();
    friend class Animation;
};
