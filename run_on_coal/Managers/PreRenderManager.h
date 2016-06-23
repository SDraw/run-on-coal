#pragma once

namespace ROC
{

class Core;
class Model;
class PreRenderManager
{
    Core *m_core;
    std::set<Model*> m_staticModelSet;
    std::set<Model*> m_animatedModelSet;
public:
    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);
protected:
    void DoPulse_S1();
    void DoPulse_S2();
    PreRenderManager(Core *f_core);
    ~PreRenderManager();
    friend Core;
    friend class ElementManager;
};

}
