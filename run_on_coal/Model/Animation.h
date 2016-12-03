#pragma once

namespace ROC
{
class Animation
{
    std::ifstream m_animFile;

    unsigned int m_bonesValue;
    unsigned int m_duration;
    unsigned int m_durationTotal;
    unsigned int m_fps;
    unsigned int m_frameDelta;

    unsigned int m_frameSize;
    unsigned int m_cachedFrame;
    std::vector<float> m_leftFrame;
    std::vector<float> m_rightFrame;

    bool m_loaded;

    void Clean();
protected:
    Animation();
    ~Animation();

    bool Load(std::string &f_path);
    bool CacheData(unsigned int f_tick, float &f_lerp);
    inline std::vector<float>& GetLeftFrameDataRef() { return m_leftFrame; }
    inline std::vector<float>& GetRightFrameDataRef() { return m_rightFrame; }

    inline unsigned int GetBonesCount() { return m_bonesValue; }
    inline unsigned int GetTotalDuration() { return m_durationTotal; }

    friend class Model;
    friend class ElementManager;
    friend class InheritanceManager;
};
}
