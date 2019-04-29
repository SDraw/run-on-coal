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
    for(auto &iter : m_boneIntervals)
    {
        for(size_t i = 0, j = iter.size(); i < j; i++)
        {
            if(i == 0U) delete iter[i].m_leftData;
            else delete iter[i].m_rightData;
        }
        iter.clear();
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
                std::vector<keyframeData> l_intervals;
                keyframeData l_interval;

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

                    if(j == 0) l_interval.m_leftData = new BoneFrameData(l_position, l_rotation, l_scale);
                    else
                    {
                        l_interval.m_rightData = new BoneFrameData(l_position, l_rotation, l_scale);
                        l_interval.m_duration = l_frameIndex*m_frameDelta - l_interval.m_startTime;
                        l_interval.m_static = l_interval.m_leftData->IsEqual(l_interval.m_rightData);
                        l_intervals.push_back(l_interval);

                        std::swap(l_interval.m_leftData, l_interval.m_rightData);
                        l_interval.m_startTime = l_frameIndex*m_frameDelta;
                        l_interval.m_rightData = nullptr;
                    }
                }
                l_intervals.shrink_to_fit();
                m_boneIntervals.push_back(l_intervals);
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

size_t ROC::Animation::GetBonesCount() const
{
    return static_cast<size_t>(m_bonesCount);
}
unsigned int ROC::Animation::GetDuration() const
{
    return m_duration;
}

void ROC::Animation::GetData(unsigned int f_tick, std::vector<Bone*> &f_bones, float f_blend)
{
    if(m_loaded)
    {
        bool l_enableBlending = (f_blend == 1.f);
        f_tick = f_tick%m_duration;

        for(size_t i = 0U; i < static_cast<size_t>(m_bonesCount); i++)
        {
            for(size_t ii = 0U, jj = m_boneIntervals[i].size(); ii < jj; ii++)
            {
                auto &l_keyframeData = m_boneIntervals[i][ii];
                if(l_keyframeData.m_startTime + l_keyframeData.m_duration >= f_tick)
                {
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
                    break;
                }
            }
        }
    }
}
