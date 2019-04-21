#pragma once
#include "Lua/LuaFunction.h"

namespace ROC
{

class AsyncTask
{
    LuaFunction m_callback;

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
    void SetLuaCallback(const LuaFunction &f_callback);
    inline const LuaFunction& GetLuaCallback() const { return m_callback; }

    virtual void Execute() = 0;
    virtual void PostExecute() = 0;

    friend class AsyncManager;
};

}
