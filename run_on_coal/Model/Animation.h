#pragma once

namespace ROC
{
class Animation
{
    std::ifstream m_animFile;

    unsigned int m_duration;
    unsigned int m_fps;

    unsigned int m_durationTotal;
    unsigned int m_frameDelta;

    unsigned int m_frameSize;

    unsigned int m_cachedFrame;

    bool m_loaded;

    void Clean();
protected:
    unsigned int m_bonesValue;
    Animation();
    ~Animation();

    std::vector<float> m_leftFrame;
    std::vector<float> m_rightFrame;

    bool Load(std::string &f_path);

    bool CacheData(unsigned int f_tick, float &f_lerp);
    friend class Model;
    friend class ElementManager;
    friend class InheritanceManager;
};
}
