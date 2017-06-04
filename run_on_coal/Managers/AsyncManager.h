#pragma once

namespace ROC
{

class Core;
class Geometry;
class LuaArguments;
class AsyncManager
{
    Core *m_core;

    struct amLoadQuery
    {
        Geometry *m_geometry = NULL;
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

    void LoadThread();
protected:
    AsyncManager(Core *f_core);
    ~AsyncManager();

    void AddGeometryToQueue(Geometry *f_geometry, const std::string &f_path);

    void DoPulse();

    friend Core;
    friend class ElementManager;
};

}
