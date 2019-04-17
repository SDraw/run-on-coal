#pragma once

namespace ROC
{

class Core;
class AsyncTask;
class Geometry;
class LuaArguments;

typedef void(*OnGeometryLoadCallback)(Geometry*, bool);

class AsyncManager final
{
    Core *m_core;

    std::atomic<bool> m_threadSwitch;
    std::thread *m_loadThread;

    std::vector<AsyncTask*> m_preparedTasks;
    std::mutex m_preparedTasksMutex;

    std::vector<AsyncTask*> m_executionTasks; // Access only from new context thread

    std::vector<AsyncTask*> m_executedTasks;
    std::mutex m_executedTasksMutex;

    LuaArguments *m_luaArguments;

    OnGeometryLoadCallback m_geometryCallback;

    void LoadThread();

    AsyncManager(const AsyncManager &that) = delete;
    AsyncManager& operator=(const AsyncManager &that) = delete;
public:
    inline void SetGeometryCallback(OnGeometryLoadCallback f_callback) { m_geometryCallback = f_callback; }
protected:
    explicit AsyncManager(Core *f_core);
    ~AsyncManager();

    void AddGeometryLoad(Geometry *f_geometry, const std::string &f_path);

    void DoPulse();

    friend class Core;
    friend class ElementManager;
};

}
