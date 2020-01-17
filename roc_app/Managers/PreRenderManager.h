#pragma once

namespace ROC
{

class Core;
class Model;
class CustomArguments;

class PreRenderManager final
{
    Core *m_core;

    std::vector<Model*> m_models;
    std::vector<Model*>::iterator m_modelsEnd;
    std::stack<Model*> m_modelsStack;

    CustomArguments *m_arguments;

    PreRenderManager(const PreRenderManager &that) = delete;
    PreRenderManager& operator=(const PreRenderManager &that) = delete;
protected:
    explicit PreRenderManager(Core *f_core);
    ~PreRenderManager();

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);

    void DoPulse_S1();
    void DoPulse_S2();

    friend class Core;
    friend class ElementManager;
};

}
