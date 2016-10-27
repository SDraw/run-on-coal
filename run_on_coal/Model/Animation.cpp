#include "stdafx.h"
#include "Model/Animation.h"
#include "Utils/Utils.h"

ROC::Animation::Animation()
{
    m_animFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    m_duration = 0U;
    m_fps = 0U;
    m_durationTotal = 0U;
    m_bonesValue = 0U;
    m_loaded = false;
}
ROC::Animation::~Animation()
{
    m_animFile.close();
}

void ROC::Animation::Clean()
{
    m_animFile.close();
    m_duration = 0U;
    m_bonesValue = 0U;
    m_fps = 0U;
    m_durationTotal = 0U;
    m_loaded = false;
}

bool ROC::Animation::Load(std::string &f_path)
{
    if(m_loaded) return false;
    bool l_result = true;

    try
    {
        m_animFile.open(f_path, std::ios::binary | std::ios::in);
        m_animFile.read((char*)&m_fps, sizeof(m_fps));
        m_animFile.read((char*)&m_duration, sizeof(m_duration));
        m_animFile.read((char*)&m_bonesValue, sizeof(m_bonesValue));

        m_durationTotal = static_cast<unsigned int>(((1.0 / static_cast<float>(m_fps)*static_cast<float>(m_duration))*1000.f));
        m_frameDelta = static_cast<unsigned int>(((1.0 / static_cast<float>(m_fps))*1000.0));

        m_frameSize = m_bonesValue * 10 * sizeof(float);
        m_rightFrame.resize(m_bonesValue * 10);
        m_leftFrame.resize(m_bonesValue * 10);
    }
    catch(std::ifstream::failure e)
    {
        Clean();
        l_result = false;
    }
    m_loaded = l_result;
    return l_result;
}

bool ROC::Animation::CacheData(unsigned int f_tick, float &f_lerp)
{
    unsigned int l_delta = f_tick%m_frameDelta;
    unsigned int l_frameL = ((f_tick - l_delta) / m_frameDelta) % m_duration;
    unsigned int l_frameR = (l_frameL + 1) % m_duration;
    f_lerp = static_cast<float>(l_delta) / static_cast<float>(m_frameDelta);

    m_animFile.seekg(12 + (l_frameL%m_duration)*m_frameSize, std::ios::beg);
    m_animFile.read((char*)m_leftFrame.data(), m_frameSize);
    m_animFile.seekg(12 + (l_frameR%m_duration)*m_frameSize, std::ios::beg);
    m_animFile.read((char*)m_rightFrame.data(), m_frameSize);
    return m_animFile.good();
}