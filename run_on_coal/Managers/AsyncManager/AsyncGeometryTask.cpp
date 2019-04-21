#include "stdafx.h"

#include "Managers/AsyncManager/AsyncGeometryTask.h"

#include "Elements/Geometry/Geometry.h"

ROC::AsyncGeometryTask::AsyncGeometryTask(const std::string &f_path)
{
    m_filePath.assign(f_path);
}
ROC::AsyncGeometryTask::~AsyncGeometryTask()
{
}

void ROC::AsyncGeometryTask::Execute()
{
    if(m_taskState == ATS_NotExecuted)
    {
        m_taskState = ATS_Executing;
        Geometry *l_geometry = new Geometry();
        if(l_geometry->Load(m_filePath))
        {
            m_taskElement = l_geometry;
            m_taskState = ATS_PostExecuting;

            glFinish();
        }
        else m_taskState = ATS_Executed;
    }
}

void ROC::AsyncGeometryTask::PostExecute()
{
    if(m_taskState == ATS_PostExecuting)
    {
        Geometry *l_geometry = reinterpret_cast<Geometry*>(m_taskElement);
        l_geometry->GenerateVAOs();
        m_taskState = ATS_Executed;
    }
}
