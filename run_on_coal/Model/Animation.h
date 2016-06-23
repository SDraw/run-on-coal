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
    std::vector<float> m_frameVector;

    bool m_loaded;

    void Clean();
protected:
    Animation();
    ~Animation();
    bool Load(std::string &f_path);
    bool GetFrameData(int f_frame, std::vector<float> &f_data);
    bool GetInterval(unsigned long f_tick, int &f_frameL, int &f_frameR, float &f_lerp);
    unsigned int GetBonesCount();
    friend class Model;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
