#include "stdafx.h"

#include "Managers/AsyncManager/AsyncTask.h"

#include "Utils/CustomData.h"

ROC::AsyncTask::AsyncTask()
{
    m_taskElement = nullptr;
    m_taskState = ATS_NotExecuted;
    m_taskResult = ATR_None;
}
ROC::AsyncTask::~AsyncTask()
{
    for(auto iter : m_taskResultData) delete iter;
    m_taskResultData.clear();
}
