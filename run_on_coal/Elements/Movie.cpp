#include "stdafx.h"
#include "Elements/Movie.h"

ROC::Movie::Movie()
{
    m_elementType = ElementType::MovieElement;

    m_movie = NULL;
}
ROC::Movie::~Movie()
{
    m_movie->
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
            m_movie = NULL;
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

void ROC::Movie::Bind()
{
    if(m_movie)
    {
        m_movie->update();
        glBindTexture(GL_TEXTURE_2D, m_movie->getCurrentImage().getNativeHandle());
    }
}
