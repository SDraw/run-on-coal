#pragma once

namespace ROC
{

class AsyncTask
{
    AsyncTask(const AsyncTask &that) = delete;
    AsyncTask& operator=(const AsyncTask &that) = delete;
public:
    enum AsyncTaskState : unsigned char
    {
        ATS_NotExecuted = 0U,
        ATS_Executing,
        ATS_PostExecuting,
        ATS_Executed
    };
protected:
    void *m_taskElement;
    unsigned char m_taskState;

    AsyncTask();
    virtual ~AsyncTask();

    inline void* GetElement() const { return m_taskElement; }

    virtual void Execute() = 0;
    virtual void PostExecute() = 0;

    friend class AsyncManager;
};

}
