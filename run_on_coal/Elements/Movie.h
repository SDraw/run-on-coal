#pragma once
#include "Elements/Drawable.h"

namespace ROC
{

class Movie final : public Drawable
{
    sfe::Movie *m_movie;
    glm::ivec2 m_size;
public:
    unsigned int GetSampleRate();
    unsigned int GetChannelCount();
    float GetFramerate();
    float GetDuration();

    void Play();
    void Pause();
    void Stop();

    float GetVolume();
    void SetVolume(float f_val);

    float GetTime();
    void SetTime(float f_val);

    inline bool IsTransparent() const { return true; }
    inline bool IsCubic() const { return false; }
    inline const glm::ivec2& GetSize() const { return m_size; }
protected:
    Movie();
    ~Movie();
    bool Load(const std::string &f_path);

    GLuint GetTextureID() const;

    void Update();
    void Bind();

    friend class ElementManager;
    friend class RenderManager;
};

}

