#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class Movie : public Drawable
{
    sfe::Movie *m_movie;
public:
    inline unsigned int GetSampleRate() { return (m_movie ? m_movie->getSampleRate() : 0U); }
    inline unsigned int GetChannelCount() { return (m_movie ? m_movie->getChannelCount() : 0U); }
    inline float GetFramerate() { return (m_movie ? m_movie->getFramerate() : -1.f); }
    inline float GetDuration() { return (m_movie ? m_movie->getDuration().asSeconds() : -1.f); }

    void Play();
    void Pause();
    void Stop();

    inline float GetVolume() { return (m_movie ? m_movie->getVolume() : -1.f); }
    void SetVolume(float f_val);

    inline float GetTime() { return (m_movie ? m_movie->getPlayingOffset().asSeconds() : -1.f); }
    void SetTime(float f_val);

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

