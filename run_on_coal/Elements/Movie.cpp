#include "stdafx.h"

#include "Elements/Movie.h"

#include "Utils/GLBinder.h"

ROC::Movie::Movie()
{
    m_elementType = ET_Movie;
    m_elementTypeName.assign("Movie");

    m_filtering = DFT_None;
    m_movie = nullptr;
    m_size = glm::ivec2(0);
}
ROC::Movie::~Movie()
{
    if(m_movie)
    {
        GLuint l_texture = m_movie->getCurrentImage().getNativeHandle();
        if(GLBinder::IsTextureBinded(l_texture)) GLBinder::ResetTexture();
        delete m_movie;
    }
}

bool ROC::Movie::Load(const std::string &f_path)
{
    if(!m_movie)
    {
        m_movie = new sfe::Movie();
        if(m_movie->openFromFile(f_path))
        {
            auto &l_texture = m_movie->getCurrentImage();
            auto l_size = l_texture.getSize();
            m_size.x = l_size.x;
            m_size.y = l_size.y;
            m_filtering = (l_texture.isSmooth() ? DFT_Linear : DFT_Nearest);
        }
        else
        {
            delete m_movie;
            m_movie = nullptr;
        }
    }
    return (m_movie != nullptr);
}

unsigned int ROC::Movie::GetSampleRate()
{
    unsigned int l_rate = 0U;
    if(m_movie) l_rate = m_movie->getSampleRate();
    return l_rate;
}
unsigned int ROC::Movie::GetChannelCount()
{
    unsigned int l_count = 0U;
    if(m_movie) l_count = m_movie->getChannelCount();
    return l_count;
}
float ROC::Movie::GetFramerate()
{
    float l_framerate = -1.f;
    if(m_movie) l_framerate = m_movie->getFramerate();
    return l_framerate;
}
float ROC::Movie::GetDuration()
{
    float l_duration = -1.f;
    if(m_movie) l_duration = m_movie->getDuration().asSeconds();
    return l_duration;
}

void ROC::Movie::Play()
{ 
    if(m_movie) m_movie->play();
}
void ROC::Movie::Pause() 
{ 
    if(m_movie) m_movie->pause();
}
void ROC::Movie::Stop()
{ 
    if(m_movie) m_movie->stop(); 
}

float ROC::Movie::GetVolume()
{
    float l_volume = -1.f;
    if(m_movie) l_volume = m_movie->getVolume();
    return l_volume;
}
void ROC::Movie::SetVolume(float f_val)
{
    if(m_movie)
    {
        btClamp(f_val, 0.f, 100.f);
        m_movie->setVolume(f_val);
    }
}

float ROC::Movie::GetTime()
{ 
    float l_time = -1.f;
    if(m_movie) l_time = m_movie->getPlayingOffset().asSeconds();
    return l_time;
}
void ROC::Movie::SetTime(float f_val)
{
    if(m_movie)
    {
        btClamp(f_val, 0.f, std::numeric_limits<float>::max());
        sf::Time l_time = sf::seconds(f_val);
        m_movie->setPlayingOffset(l_time);
    }
}

void ROC::Movie::Update()
{
    if(m_movie) m_movie->update();
}
void ROC::Movie::Bind()
{
    if(m_movie)
    {
        GLuint l_texture = m_movie->getCurrentImage().getNativeHandle();
        GLBinder::BindTexture2D(l_texture);
    }
}
