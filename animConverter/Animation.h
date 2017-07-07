#pragma once

class Animation
{
    struct keyframeData
    {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
        int m_frameIndex;
    };
    struct frameData
    {
        std::vector<keyframeData> m_keyframes;
    };
    std::vector<frameData> m_bones;
    unsigned int m_duration;
    unsigned int m_fps;
    unsigned int m_bonesCount;

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
    bool Load(const std::string &f_path);
    bool Generate(const std::string &f_path);
};
