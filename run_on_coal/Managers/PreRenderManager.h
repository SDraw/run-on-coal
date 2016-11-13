#pragma once

namespace ROC
{
class Core;
class PhysicsManager;
class Model;
class LuaArguments;
class PreRenderManager
{
    Core *m_core;
    PhysicsManager *m_physicsManager;

    std::set<Model*> m_staticModelSet;
    std::set<Model*> m_animatedModelSet;

    LuaArguments *m_argument;

    PreRenderManager(const PreRenderManager& that);
    PreRenderManager &operator =(const PreRenderManager &that);
public:
    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);
protected:
    explicit PreRenderManager(Core *f_core);
    ~PreRenderManager();

    void DoPulse_S1();
    void DoPulse_S2();
    friend Core;
    friend class ElementManager;
};
}
