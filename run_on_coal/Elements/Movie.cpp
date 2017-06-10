#include "stdafx.h"
#include "Elements/Movie.h"

ROC::Movie::Movie()
{
    m_elementType = ElementType::MovieElement;

    m_movie = nullptr;
}
ROC::Movie::~Movie()
{
    if(m_movie) delete m_movie;
}

bool ROC::Movie::Load(const std::string &f_path)
{
    bool l_result = false;
    if(!m_movie)
    {
        m_movie = new sfe::Movie();
        l_result = m_movie->openFromFile(f_path);
        if(!l_result)
        {
            delete m_movie;
            m_movie = nullptr;
        }
    }
    return l_result;
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

void ROC::Movie::SetVolume(float f_val)
{
    if(m_movie)
    {
        btClamp(f_val, 0.f, 100.f);
        m_movie->setVolume(f_val);
    }
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
    if(m_movie) glBindTexture(GL_TEXTURE_2D, m_movie->getCurrentImage().getNativeHandle());
}
