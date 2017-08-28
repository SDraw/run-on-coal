#include "stdafx.h"

#include "Elements/Movie.h"

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
    delete m_movie;
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

void ROC::Movie::SetVolume(float f_val)
{
    btClamp(f_val, 0.f, 100.f);
    m_movie->setVolume(f_val);
}

void ROC::Movie::SetTime(float f_val)
{
    btClamp(f_val, 0.f, std::numeric_limits<float>::max());
    sf::Time l_time = sf::seconds(f_val);
    m_movie->setPlayingOffset(l_time);
}
