#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Bone;
class BoneFrameData;
class Animation final : public Element
{
    unsigned int m_bonesCount;
    unsigned int m_framesCount;
    unsigned int m_duration; // Animation duration in milliseconds
    unsigned int m_fps;
    unsigned int m_frameDelta;

    struct keyframeData
    {
        BoneFrameData *m_leftData = nullptr;
        BoneFrameData *m_rightData = nullptr;
        unsigned int m_startTime = 0U;
        unsigned int m_duration = 0U; // Duration between left and right keyframes in milliseconds
    };
    std::vector<IntervalTree<keyframeData>*> m_boneIntervals;
    BoneFrameData *m_tempFrameData;
    std::vector<Interval<keyframeData>> m_searchResult;

    bool m_loaded;

    void Clean();
public:
    inline unsigned int GetBonesCount() const { return m_bonesCount; }
    inline unsigned int GetDuration() const { return m_duration; }
    void GetData(unsigned int f_tick, std::vector<Bone*> &f_bones);
protected:
    Animation();
    ~Animation();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
