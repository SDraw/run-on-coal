#include "stdafx.h"

#include "Managers/AsyncManager/AsyncGeometryTask.h"

#include "Elements/Geometry/Geometry.h"

ROC::AsyncGeometryTask::AsyncGeometryTask(Geometry *f_geometry, const std::string &f_path)
{
    m_taskType = ATT_Geometry;
    m_taskElement = f_geometry;
    m_filePath.assign(f_path);
}
ROC::AsyncGeometryTask::~AsyncGeometryTask()
{
}

void ROC::AsyncGeometryTask::Execute()
{
    m_taskState = ATS_Executing;
    Geometry *l_geometry = dynamic_cast<Geometry*>(m_taskElement);
    if(l_geometry)
    {
        if(l_geometry->Load(m_filePath)) m_taskState = ATS_PostExecuting;
        else
        {
            m_taskResult = ATR_Fail;
            m_taskState = ATS_Executed;
        }
        glFinish();
    }
    else
    {
        m_taskResult = ATR_Fail;
        m_taskState = ATS_Executed;
    }
}

void ROC::AsyncGeometryTask::PostExecute()
{
    if(m_taskState == ATS_PostExecuting)
    {
        Geometry *l_geometry = dynamic_cast<Geometry*>(m_taskElement);
        if(l_geometry)
        {
            l_geometry->GenerateVAOs();
            m_taskResult = ATR_Success;
        }
        else m_taskResult = ATR_Fail;
        m_taskState = ATS_Executed;
    }
}
