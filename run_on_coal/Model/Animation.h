#pragma once

namespace ROC
{

class Animation
{
    std::ifstream m_animFile;

    unsigned int m_duration;
    unsigned int m_fps;
    unsigned int m_bonesValue;

    unsigned long m_durationTotal;
    unsigned long m_frameDelta;

    unsigned long m_frameSize;

    bool m_loaded;

    void Clean();
protected:
    Animation();
    ~Animation();
    std::vector<float> m_leftFrame;
    std::vector<float> m_rightFrame;
    bool Load(std::string &f_path);
    void GetInterval(unsigned long f_tick, unsigned int &f_frameL, unsigned int &f_frameR, float &f_lerp);
    bool GetFrameData(unsigned int f_rightFrame, unsigned int f_leftFrame);
    unsigned int GetBonesCount();
    friend class Model;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
