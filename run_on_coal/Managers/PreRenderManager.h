#pragma once

namespace ROC
{
class Core;
class EventManager;
class PhysicsManager;
class Model;
class LuaArguments;
class PreRenderManager
{
    Core *m_core;
    EventManager *m_eventManager;
    PhysicsManager *m_physicsManager;

    struct TreeNode
    {
        Model *m_model;
        TreeNode *m_parent = NULL;
        std::set<TreeNode*> m_children;
    };
    std::set<TreeNode*> m_modelTreeSet;
    std::map<Model*, TreeNode*> m_modelToNodeMap;
    std::list<TreeNode*> m_nodeList;

    std::set<Model*> m_animatedModelSet;

    LuaArguments *m_argument;

    PreRenderManager(const PreRenderManager& that);
    PreRenderManager &operator =(const PreRenderManager &that);
protected:
    explicit PreRenderManager(Core *f_core);
    ~PreRenderManager();

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);

    void AddLink(Model *f_model, Model *f_parent);
    void RemoveLink(Model *f_model);

    void DoPulse_S1();
    void DoPulse_S2();
    friend Core;
    friend class ElementManager;
    friend class InheritanceManager;
};
}
