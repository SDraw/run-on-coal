#pragma once
#include "Elements/Element.h"

namespace ROC
{

class BoneFrameData;
class Animation : public Element
{
    std::ifstream m_animFile;

    unsigned int m_bonesValue;
    unsigned int m_duration;
    unsigned int m_durationTotal;
    unsigned int m_fps;
    unsigned int m_frameDelta;
    unsigned int m_frameSize;
    unsigned int m_lastLeftFrame;
    std::vector<BoneFrameData*> m_leftFrame;
    std::vector<BoneFrameData*> m_rightFrame;
    std::vector<BoneFrameData*> m_interpolatedFrame;

    bool m_loaded;

    void Clean();
protected:
    Animation();
    ~Animation();

    bool Load(const std::string &f_path);
    bool CacheData(unsigned int f_tick);
    inline std::vector<BoneFrameData*>& GetCachedDataRef() { return m_interpolatedFrame; }

    inline unsigned int GetBonesCount() const { return m_bonesValue; }
    inline unsigned int GetTotalDuration() const { return m_durationTotal; }

    friend class Model;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
