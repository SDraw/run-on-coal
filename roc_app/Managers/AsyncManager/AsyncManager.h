#pragma once
#include "Interfaces/IAsyncManager.h"

namespace ROC
{

class Core;
class AsyncTask;
class CustomArguments;

class AsyncManager final : public IAsyncManager
{
    Core *m_core;

    std::atomic<bool> m_threadSwitch;
    std::thread *m_loadThread;

    std::vector<AsyncTask*> m_preparedTasks;
    std::mutex m_preparedTasksMutex;

    std::vector<AsyncTask*> m_executionTasks; // Access only from new context thread

    std::vector<AsyncTask*> m_executedTasks;
    std::mutex m_executedTasksMutex;

    CustomArguments *m_arguments;

    void ExecutionThread();

    AsyncManager(const AsyncManager &that) = delete;
    AsyncManager& operator=(const AsyncManager &that) = delete;
public:
    void* LoadGeometry(const std::string &f_path);
    void* LoadTexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress);
    void* LoadTexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
protected:
    explicit AsyncManager(Core *f_core);
    ~AsyncManager();

    void DoPulse();

    friend class Core;
};

}
