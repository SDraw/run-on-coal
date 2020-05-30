#include "stdafx.h"

#include "Managers/AsyncManager/AsyncTask.h"

ROC::AsyncTask::AsyncTask()
{
    m_taskElement = nullptr;
    m_taskState = ATS_NotExecuted;
}

ROC::AsyncTask::~AsyncTask()
{
}
