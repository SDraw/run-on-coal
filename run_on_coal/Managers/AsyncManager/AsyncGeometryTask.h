#pragma once
#include "Managers/AsyncManager/AsyncTask.h"

namespace ROC
{
class Geometry;

class AsyncGeometryTask final : public AsyncTask
{
    std::string m_filePath;

    AsyncGeometryTask(const AsyncGeometryTask &that) = delete;
    AsyncGeometryTask& operator=(const AsyncGeometryTask &that) = delete;
protected:
    explicit AsyncGeometryTask(Geometry *f_geometry, const std::string &f_path);
    ~AsyncGeometryTask();

    void Execute();
    void PostExecute();

    friend class AsyncManager;
};

}
