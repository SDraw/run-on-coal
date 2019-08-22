#include "stdafx.h"

#include "Managers/AsyncManager/AsyncTextureTask.h"

#include "Elements/Texture.h"

ROC::AsyncTextureTask::AsyncTextureTask(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress)
{
    m_path.push_back(f_path);
    m_type = f_type;
    m_filter = f_filter;
    m_compress = f_compress;
}
ROC::AsyncTextureTask::AsyncTextureTask(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress)
{
    m_path.assign(f_path.begin(), f_path.end());
    m_filter = f_filter;
    m_compress = f_compress;
}
ROC::AsyncTextureTask::~AsyncTextureTask()
{
}

void ROC::AsyncTextureTask::Execute()
{
    if(m_taskState == ATS_NotExecuted)
    {
        m_taskState = ATS_Executing;

        Texture *l_texture = new Texture();
        switch(m_path.size())
        {
            case 1U:
            {
                if(l_texture->Load(m_path[0U], m_type, m_filter, m_compress)) m_taskElement = l_texture;
                else delete l_texture;
            } break;
            case 6U:
            {
                if(l_texture->LoadCubemap(m_path, m_filter, m_compress)) m_taskElement = l_texture;
                else delete l_texture;
            } break;
        }

        m_taskState = ATS_Executed;
    }
}

void ROC::AsyncTextureTask::PostExecute()
{
}
