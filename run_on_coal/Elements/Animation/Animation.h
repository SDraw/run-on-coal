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
        bool m_static = false;
    };
    std::vector<std::vector<keyframeData>> m_boneIntervals;
    BoneFrameData *m_tempFrameData;

    bool m_loaded;

    Animation(const Animation &that);
    Animation& operator=(const Animation &that);

    void Clean();
public:
    inline size_t GetBonesCount() const { return static_cast<size_t>(m_bonesCount); }
    inline unsigned int GetDuration() const { return m_duration; }
    void GetData(unsigned int f_tick, std::vector<Bone*> &f_bones, float f_blend = 1.f);
protected:
    Animation();
    ~Animation();
    bool Load(const std::string &f_path);

    friend class ElementManager;
};

}
