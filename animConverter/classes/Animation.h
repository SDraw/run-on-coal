#pragma once

class Frame;
class Animation
{
    std::vector<Frame*> m_frameVector;
    unsigned int m_duration;
    unsigned int m_fps;
    unsigned int m_bonesValue;

    bool m_loaded;

    void Clean();

    struct boneData
    {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
    };
public:
    Animation();
    ~Animation();
    bool LoadFromJSON(std::string &f_path);
    bool GenerateBinary(std::string &f_path);
};
