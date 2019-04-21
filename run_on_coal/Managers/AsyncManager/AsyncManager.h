#pragma once
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class AsyncTask;
class Element;
class LuaFunction;

typedef void(*OnAsyncTaskFinish)(void*, Element*);

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

    CustomArguments m_luaArguments;

    OnAsyncTaskFinish m_asyncTaskCallback;

    void ExecutionThread();

    AsyncManager(const AsyncManager &that) = delete;
    AsyncManager& operator=(const AsyncManager &that) = delete;
public:
    inline void SetAsyncTaskCallback(OnAsyncTaskFinish f_callback) { m_asyncTaskCallback = f_callback; }

    void* LoadGeometry(const std::string &f_path, const LuaFunction &f_callback);
    void* LoadGeometry(const std::string &f_path);

    void* LoadTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress, const LuaFunction &f_callback);
    void* LoadTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress, const LuaFunction &f_callback);
    void* LoadTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress);
    void* LoadTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress);
protected:
    explicit AsyncManager(Core *f_core);
    ~AsyncManager();

    void DoPulse();

    friend class Core;
};

}
