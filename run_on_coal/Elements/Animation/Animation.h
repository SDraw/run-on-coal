#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Bone;
class BoneFrameData;
class Animation : public Element
{
    unsigned int m_bonesValue;
    unsigned int m_duration;
    unsigned int m_durationTotal;
    unsigned int m_fps;
    unsigned int m_frameDelta;

    struct animData
    {
        BoneFrameData *m_leftData = NULL;
        BoneFrameData *m_rightData = NULL;
        unsigned int m_leftTime = 0U;
        unsigned int m_duration = 0U;
    };
    std::vector<IntervalTree<animData>*> m_boneIntervals;
    BoneFrameData *m_tempFrameData;
    std::vector<Interval<animData>> m_searchResult;

    bool m_loaded;

    void Clean();
public:
    inline unsigned int GetBonesCount() const { return m_bonesValue; }
    inline unsigned int GetTotalDuration() const { return m_durationTotal; }
    void GetData(unsigned int f_tick, std::vector<Bone*> &f_bones);
protected:
    Animation();
    ~Animation();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
