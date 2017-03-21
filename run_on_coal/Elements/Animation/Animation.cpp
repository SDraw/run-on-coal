#include "stdafx.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Animation/BoneFrameData.h"
#include "Elements/Model/Bone.h"
#include "Utils/Utils.h"

ROC::Animation::Animation()
{
    m_elementType = ElementType::AnimationElement;

    m_duration = 0U;
    m_durationTotal = 0U;
    m_fps = 0U;
    m_frameDelta = 0U;
    m_tempFrameData = NULL;
    m_bonesValue = 0U;
    m_loaded = false;
}
ROC::Animation::~Animation()
{
    Clean();
}

void ROC::Animation::Clean()
{
    m_duration = 0U;
    m_bonesValue = 0U;
    m_fps = 0U;
    m_durationTotal = 0U;
    if(m_tempFrameData) delete m_tempFrameData;
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
            l_animFile.read(reinterpret_cast<char*>(&m_duration), sizeof(unsigned int));
            l_animFile.read(reinterpret_cast<char*>(&m_bonesValue), sizeof(unsigned int));

            m_durationTotal = static_cast<unsigned int>((static_cast<float>(m_duration) / static_cast<float>(m_fps))*1000.f);
            m_frameDelta = static_cast<unsigned int>((1.0 / static_cast<float>(m_fps))*1000.0);

            for(unsigned int i = 0; i < m_bonesValue; i++)
            {
                std::vector<Interval<animData>> l_intervals;
                Interval<animData> l_interval(0, 0, animData());

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
                        l_interval.start = l_frameIndex;
                        l_interval.value.m_leftData = new BoneFrameData();
                        l_interval.value.m_leftData->m_position = l_position;
                        l_interval.value.m_leftData->m_rotation = l_rotation;
                        l_interval.value.m_leftData->m_scale = l_scale;
                    }
                    else
                    {
                        l_interval.stop = l_frameIndex;
                        l_interval.value.m_rightData = new BoneFrameData();
                        l_interval.value.m_rightData->m_position = l_position;
                        l_interval.value.m_rightData->m_rotation = l_rotation;
                        l_interval.value.m_rightData->m_scale = l_scale;
                        l_interval.value.m_duration = l_frameIndex*m_frameDelta - l_interval.value.m_leftTime;

                        l_intervals.push_back(l_interval);

                        l_interval.start = l_frameIndex;
                        l_interval.value.m_leftData = l_interval.value.m_rightData;
                        l_interval.value.m_leftTime = l_frameIndex*m_frameDelta;
                        l_interval.value.m_rightData = NULL;
                    }
                }
                m_boneIntervals.push_back(new IntervalTree<animData>(l_intervals));
            }
            l_animFile.close();

            m_tempFrameData = new BoneFrameData();
            m_loaded = true;
        }
        catch(const std::ifstream::failure&)
        {
            Clean();
        }
    }
    return m_loaded;
}

void ROC::Animation::GetData(unsigned int f_tick, std::vector<Bone*> &f_bones)
{
    unsigned int l_frame = ((f_tick - f_tick%m_frameDelta) / m_frameDelta) % m_duration;
    f_tick = f_tick%m_durationTotal;

    for(unsigned int i = 0; i < m_bonesValue; i++)
    {
        m_boneIntervals[i]->findOverlapping(l_frame, l_frame, m_searchResult);
        if(!m_searchResult.empty())
        {
            animData &l_animData = m_searchResult.back().value;
            float l_lerp = Utils::EaseInOut(static_cast<float>(f_tick - l_animData.m_leftTime) / static_cast<float>(l_animData.m_duration));
            m_tempFrameData->m_position = glm::lerp(l_animData.m_leftData->m_position, l_animData.m_rightData->m_position, l_lerp);
            m_tempFrameData->m_rotation = glm::slerp(l_animData.m_leftData->m_rotation, l_animData.m_rightData->m_rotation, l_lerp);
            m_tempFrameData->m_scale = glm::lerp(l_animData.m_leftData->m_scale, l_animData.m_rightData->m_scale, l_lerp);
            f_bones[i]->SetFrameData(m_tempFrameData);

            m_searchResult.clear();
        }
    }
}
