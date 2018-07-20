#include "stdafx.h"

#include "Elements/Animation/Animation.h"
#include "Elements/Animation/BoneFrameData.h"

#include "Elements/Model/Bone.h"
#include "Utils/MathUtils.h"

ROC::Animation::Animation()
{
    m_elementType = ET_Animation;
    m_elementTypeName.assign("Animation");

    m_framesCount = 0U;
    m_duration = 0U;
    m_fps = 0U;
    m_frameDelta = 0U;
    m_tempFrameData = nullptr;
    m_bonesCount = 0U;
    m_loaded = false;
}
ROC::Animation::~Animation()
{
    Clean();
}

void ROC::Animation::Clean()
{
    m_bonesCount = 0U;
    m_framesCount = 0U;
    m_duration = 0U;
    m_fps = 0U;
    delete m_tempFrameData;
    for(auto iter : m_boneIntervals)
    {
        auto &l_intervals = iter->intervals;
        for(size_t i = 0, j = l_intervals.size(); i < j; i++)
        {
            if(i == 0U) delete l_intervals[i].value.m_leftData;
            else delete l_intervals[i].value.m_rightData;
        }
        delete iter;
    }
    m_boneIntervals.clear();
    m_loaded = false;
}

bool ROC::Animation::Load(const std::string &f_path)
{
    if(!m_loaded)
    {
        std::ifstream l_animFile;
        l_animFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            l_animFile.open(f_path, std::ios::binary | std::ios::in);
            l_animFile.read(reinterpret_cast<char*>(&m_fps), sizeof(unsigned int));
            l_animFile.read(reinterpret_cast<char*>(&m_framesCount), sizeof(unsigned int));
            l_animFile.read(reinterpret_cast<char*>(&m_bonesCount), sizeof(unsigned int));

            m_frameDelta = 1000U / m_fps;
            m_duration = m_framesCount * m_frameDelta;

            for(unsigned int i = 0; i < m_bonesCount; i++)
            {
                std::vector<Interval<keyframeData>> l_intervals;
                Interval<keyframeData> l_interval(0, 0, keyframeData());

                int l_intervalsCount = 0;
                l_animFile.read(reinterpret_cast<char*>(&l_intervalsCount), sizeof(int));

                for(int j = 0; j < l_intervalsCount; j++)
                {
                    glm::vec3 l_position;
                    glm::quat l_rotation;
                    glm::vec3 l_scale;
                    int l_frameIndex;
                    l_animFile.read(reinterpret_cast<char*>(&l_position), sizeof(glm::vec3));
                    l_animFile.read(reinterpret_cast<char*>(&l_rotation), sizeof(glm::quat));
                    l_animFile.read(reinterpret_cast<char*>(&l_scale), sizeof(glm::vec3));
                    l_animFile.read(reinterpret_cast<char*>(&l_frameIndex), sizeof(int));

                    if(j == 0)
                    {
                        l_interval.start = static_cast<size_t>(l_frameIndex);
                        l_interval.value.m_leftData = new BoneFrameData(l_position, l_rotation, l_scale);
                    }
                    else
                    {
                        l_interval.stop = static_cast<size_t>(l_frameIndex);

                        auto &l_value = l_interval.value;
                        l_value.m_rightData = new BoneFrameData(l_position, l_rotation, l_scale);
                        l_value.m_duration = l_frameIndex*m_frameDelta - l_value.m_startTime;
                        l_value.m_static = l_value.m_leftData->IsEqual(l_value.m_rightData);
                        l_intervals.push_back(l_interval);

                        l_interval.start = static_cast<size_t>(l_frameIndex);
                        l_value.m_leftData = l_value.m_rightData;
                        l_value.m_startTime = l_frameIndex*m_frameDelta;
                        l_value.m_rightData = nullptr;
                    }
                }
                l_intervals.shrink_to_fit();
                m_boneIntervals.push_back(new IntervalTree<keyframeData>(l_intervals));
            }
            m_boneIntervals.shrink_to_fit();
            l_animFile.close();

            m_tempFrameData = new BoneFrameData();
            m_loaded = true;
        }
        catch(const std::exception&)
        {
            Clean();
        }
    }
    return m_loaded;
}

void ROC::Animation::GetData(unsigned int f_tick, std::vector<Bone*> &f_bones, float f_blend)
{
    if(m_loaded)
    {
        bool l_enableBlending = (f_blend == 1.f);
        size_t l_frame = static_cast<size_t>(((f_tick - f_tick%m_frameDelta) / m_frameDelta) % m_framesCount);
        f_tick = f_tick%m_duration;

        for(size_t i = 0; i < static_cast<size_t>(m_bonesCount); i++)
        {
            m_boneIntervals[i]->findOverlapping(l_frame, l_frame, m_searchResult);
            if(!m_searchResult.empty())
            {
                keyframeData &l_keyframeData = m_searchResult.back().value;
                if(l_keyframeData.m_static)
                {
                    f_bones[i]->SetFrameData(l_keyframeData.m_leftData);
                    l_enableBlending ? f_bones[i]->SetFrameData(l_keyframeData.m_leftData) : f_bones[i]->SetFrameData(l_keyframeData.m_leftData, f_blend);
                }
                else
                {
                    float l_blend = MathUtils::EaseInOut(static_cast<float>(f_tick - l_keyframeData.m_startTime) / static_cast<float>(l_keyframeData.m_duration));
                    m_tempFrameData->SetInterpolated(l_keyframeData.m_leftData, l_keyframeData.m_rightData, l_blend);
                    l_enableBlending ? f_bones[i]->SetFrameData(m_tempFrameData) : f_bones[i]->SetFrameData(m_tempFrameData, f_blend);
                }

                m_searchResult.clear();
            }
        }
    }
}
