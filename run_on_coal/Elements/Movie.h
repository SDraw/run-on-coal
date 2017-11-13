#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class Movie final : public Drawable
{
    sfe::Movie *m_movie;
    glm::ivec2 m_size;
public:
    inline unsigned int GetSampleRate() { return m_movie->getSampleRate(); }
    inline unsigned int GetChannelCount() { return m_movie->getChannelCount(); }
    inline float GetFramerate() { return m_movie->getFramerate(); }
    inline float GetDuration() { return m_movie->getDuration().asSeconds(); }

    inline void Play() { m_movie->play(); }
    inline void Pause() { m_movie->pause(); }
    inline void Stop() { m_movie->stop(); }

    inline float GetVolume() { return m_movie->getVolume(); }
    void SetVolume(float f_val);

    inline float GetTime() { return m_movie->getPlayingOffset().asSeconds(); }
    void SetTime(float f_val);

    inline bool IsTransparent() const { return true; }
    inline bool IsCubic() const { return false; }
    inline const glm::ivec2& GetSize() const { return m_size; }
protected:
    Movie();
    ~Movie();
    bool Load(const std::string &f_path);

    inline void Update() { m_movie->update(); }
    void Bind();

    friend class ElementManager;
    friend class RenderManager;
};

}

