#pragma once

namespace ROC
{

class Core;
class Geometry;
class LuaArguments;
typedef void(*OnGeometryLoadCallback)(Geometry*, bool);

class AsyncManager final
{
    Core *m_core;

    struct amLoadQuery
    {
        Geometry *m_geometry = nullptr;
        std::string m_path;
        bool m_result = false;
    };

    std::atomic<bool> m_threadSwitch;
    std::thread *m_loadThread;

    std::vector<amLoadQuery> m_loadingQueue;
    std::mutex m_loadingMutex;

    std::vector<amLoadQuery> m_loadedQueue;
    std::mutex m_loadedMutex;

    LuaArguments *m_argument;

    OnGeometryLoadCallback m_callback;

    void LoadThread();

    AsyncManager(const AsyncManager& that);
    AsyncManager &operator =(const AsyncManager &that);
public:
    inline void SetCallback(OnGeometryLoadCallback f_callback) { m_callback = f_callback; }
protected:
    explicit AsyncManager(Core *f_core);
    ~AsyncManager();

    void AddGeometryToQueue(Geometry *f_geometry, const std::string &f_path);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
};

}
