#pragma once

namespace ROC
{

class Element;
class CustomData;

class AsyncTask
{
    AsyncTask(const AsyncTask &that) = delete;
    AsyncTask& operator=(const AsyncTask &that) = delete;
public:
    enum AsyncTaskType : unsigned char
    {
        ATT_Geometry = 0U
    };
    enum AsyncTaskState : unsigned char
    {
        ATS_NotExecuted = 0U,
        ATS_Executing,
        ATS_PostExecuting,
        ATS_Executed
    };
    enum AsyncTaskResult : unsigned char
    {
        ATR_None = 0U,
        ATR_Success,
        ATR_Fail
    };
protected:
    Element *m_taskElement;
    AsyncTaskType m_taskType;
    AsyncTaskState m_taskState;
    AsyncTaskResult m_taskResult;
    std::vector<CustomData*> m_taskResultData;

    AsyncTask();
    virtual ~AsyncTask();

    virtual void Execute() = 0;
    virtual void PostExecute() = 0;

    inline AsyncTaskType GetType() const { return m_taskType; }
    inline Element* GetElement() const { return m_taskElement; }
    inline AsyncTaskResult GetResult() const { return m_taskResult; }
    inline const std::vector<CustomData*>& GetResultData() const { return m_taskResultData; }

    friend class AsyncManager;
};

}
