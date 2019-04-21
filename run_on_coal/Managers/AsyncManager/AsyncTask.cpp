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

void ROC::AsyncTask::SetLuaCallback(const LuaFunction &f_callback)
{
    m_callback = f_callback;
}
