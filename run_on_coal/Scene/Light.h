#pragma once
namespace ROC
{

class Light
{
    glm::vec3 m_direction;
    glm::vec3 m_color;
    glm::vec4 m_params;
public:
    void SetParams(glm::vec4 &f_val);
    void GetParams(glm::vec4 &f_val);

    void SetColor(glm::vec3 &f_vec);
    void GetColor(glm::vec3 &f_vec);

    void SetDirection(glm::vec3 &f_vec);
    void GetDirection(glm::vec3 &f_vec);
protected:
    Light();
    ~Light();
    friend class ElementManager;
};

}
