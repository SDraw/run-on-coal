#pragma once
namespace ROC
{
class Light
{
public:
    inline void SetParams(glm::vec4 &f_val) { std::memcpy(&m_params, &f_val, sizeof(glm::vec4)); }
    inline void GetParams(glm::vec4 &f_val) { std::memcpy(&f_val, &m_params, sizeof(glm::vec4)); }

    inline void SetColor(glm::vec3 &f_vec) { std::memcpy(&m_color, &f_vec, sizeof(glm::vec3)); }
    inline void GetColor(glm::vec3 &f_vec) { std::memcpy(&f_vec, &m_color, sizeof(glm::vec3)); }

    inline void SetDirection(glm::vec3 &f_vec) { std::memcpy(&m_direction, &f_vec, sizeof(glm::vec3)); }
    inline void GetDirection(glm::vec3 &f_vec) { std::memcpy(&f_vec, &m_direction, sizeof(glm::vec3)); }
protected:
    glm::vec3 m_direction;
    glm::vec3 m_color;
    glm::vec4 m_params;

    Light();
    ~Light();
    friend class ElementManager;
    friend class RenderManager;
};
}
