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

    m_durationTotal = unsigned(long((1.0/double(m_fps)*double(m_duration))*1000.0));
    m_frameDelta = unsigned(long((1.0/double(m_fps))*1000.0));

    m_frameSize = m_bonesValue*10*sizeof(float);
    m_frameVector.resize(m_bonesValue*10);

    m_loaded = true;
    return true;
}

bool ROC::Animation::GetFrameData(int f_frame, std::vector<float> &f_data)
{
    if(f_frame < 0 || f_frame >= int(m_duration)) return false;
    m_animFile.seekg(12+f_frame*m_frameSize,std::ios::beg);
    if(!m_animFile.good()) return false;
    m_animFile.read((char*)m_frameVector.data(),m_frameSize);
    if(!m_animFile.good()) return false;
    f_data.insert(f_data.begin(),m_frameVector.begin(),m_frameVector.end());
    return true;
}

bool ROC::Animation::GetInterval(unsigned long f_tick, int &f_frameL, int &f_frameR, float &f_lerp)
{
    unsigned long l_delta = f_tick%m_frameDelta;
    f_lerp = float(l_delta)/float(m_frameDelta);
    f_frameL = ((f_tick-l_delta)/m_frameDelta)%m_duration;
    f_frameR = (f_frameL+1)%m_duration;
    return true;
}

unsigned int ROC::Animation::GetBonesCount()
{
    return m_bonesValue;
}
