#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class Movie : public Drawable
{
    sfe::Movie *m_movie;
public:
    void Play();
    void Pause();
    void Stop();

    inline bool IsTransparent() const { return true; }
protected:
    Movie();
    ~Movie();
    bool Load(const std::string &f_path);

    inline GLuint GetTextureID() const { return (m_movie ? m_movie->getCurrentImage().getNativeHandle() : 0U); }

    void Bind();

    friend class ElementManager;
};

}

