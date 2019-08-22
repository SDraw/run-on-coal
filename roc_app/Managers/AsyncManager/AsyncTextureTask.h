#pragma once
#include "Managers/AsyncManager/AsyncTask.h"

namespace ROC
{

class AsyncTextureTask : public AsyncTask
{
    std::vector<std::string> m_path;
    unsigned char m_type;
    unsigned char m_filter;
    bool m_compress;
protected:
    AsyncTextureTask(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress);
    AsyncTextureTask(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
    ~AsyncTextureTask();

    void Execute();
    void PostExecute();

    friend class AsyncManager;
};

}
