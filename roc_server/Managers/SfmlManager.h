#pragma once
#define CURSOR_LOCK_BIT 1U
#define CURSOR_VISIBILITY_BIT 2U

namespace ROC
{

class Core;
class LuaArguments;
class SfmlManager
{
    Core *m_core;

    float m_time;
    sf::Clock m_clock;

    SfmlManager(const SfmlManager& that);
    SfmlManager &operator =(const SfmlManager &that);
public:
    inline float GetTime() const { return m_time; }
protected:
    explicit SfmlManager(Core *f_core);
    ~SfmlManager();

    void DoPulse();

    friend Core;
};

}
