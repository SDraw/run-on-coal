#include "stdafx.h"
#include "Model/Animation.h"
#include "Utils/Utils.h"

ROC::Animation::Animation()
{
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

#define AnimationCleanAfterFail() { Clean(); return false; }
bool ROC::Animation::Load(std::string &f_path)
{
    if(m_loaded) return false;
    m_animFile.open(f_path,std::ios::binary|std::ios::in);
    if(m_animFile.fail()) return false;
    m_animFile.read((char*)&m_fps,sizeof(m_fps));
    if(!m_animFile.good()) AnimationCleanAfterFail();
    m_animFile.read((char*)&m_duration,sizeof(m_duration));
    if(!m_animFile.good()) AnimationCleanAfterFail();
    m_animFile.read((char*)&m_bonesValue,sizeof(m_bonesValue));
    if(!m_animFile.good()) AnimationCleanAfterFail();

    m_durationTotal = static_cast<unsigned long>(((1.0/static_cast<double>(m_fps)*static_cast<double>(m_duration))*1000.0));
    m_frameDelta = static_cast<unsigned long>(((1.0/static_cast<double>(m_fps))*1000.0));

    m_frameSize = m_bonesValue*10*sizeof(float);
    m_rightFrame.resize(m_bonesValue*10);
    m_leftFrame.resize(m_bonesValue*10);

    m_loaded = true;
    return true;
}

bool ROC::Animation::GetFrameData(unsigned int f_rightFrame, unsigned int f_leftFrame)
{
    m_animFile.seekg(12+(f_leftFrame%m_duration)*m_frameSize,std::ios::beg);
    m_animFile.read((char*)m_leftFrame.data(),m_frameSize);
    m_animFile.seekg(12+(f_rightFrame%m_duration)*m_frameSize,std::ios::beg);
    m_animFile.read((char*)m_rightFrame.data(),m_frameSize);
    return m_animFile.good();
}

void ROC::Animation::GetInterval(unsigned long f_tick, unsigned int &f_frameL, unsigned int &f_frameR, float &f_lerp)
{
    unsigned long l_delta = f_tick%m_frameDelta;
    f_lerp = static_cast<float>(l_delta)/static_cast<float>(m_frameDelta);
    f_frameL = ((f_tick-l_delta)/m_frameDelta)%m_duration;
    f_frameR = (f_frameL+1)%m_duration;
}
