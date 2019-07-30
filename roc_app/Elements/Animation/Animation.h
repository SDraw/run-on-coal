#pragma once
#include "Interfaces/IAnimation.h"
#include "Elements/Element.h"

namespace ROC
{

class Bone;
class BoneFrameData;

class Animation final : public Element, public virtual IAnimation
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
        //unsigned char m_interpolationType; // TODO
    };
    std::vector<std::vector<keyframeData>> m_boneIntervals;
    BoneFrameData *m_tempFrameData;

    bool m_loaded;

    Animation(const Animation &that) = delete;
    Animation& operator=(const Animation &that) = delete;

    void Clean();
public:
    size_t GetBonesCount() const;
    unsigned int GetDuration() const;
protected:
    Animation();
    ~Animation();

    bool Load(const std::string &f_path);

    void GetData(unsigned int f_tick, std::vector<Bone*> &f_bones, float f_blend = 1.f);

    friend class ElementManager;
    friend class Model;
    friend class Skeleton;
};

}
