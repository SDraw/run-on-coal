#pragma once
#include "Utils/CustomArguments.h"

namespace ROC
{

class Core;
class Model;
class TreeNode;

class PreRenderManager final
{
    Core *m_core;

    TreeNode *m_modelTreeRoot;
    std::map<Model*, TreeNode*> m_modelToNodeMap;
    std::map<Model*, TreeNode*>::iterator m_modelToNodeMapEnd;

    std::vector<TreeNode*> m_nodeStack;

    CustomArguments m_arguments;

    PreRenderManager(const PreRenderManager &that) = delete;
    PreRenderManager& operator=(const PreRenderManager &that) = delete;
protected:
    explicit PreRenderManager(Core *f_core);
    ~PreRenderManager();

    void AddModel(Model *f_model);
    void RemoveModel(Model *f_model);

    void AddLink(Model *f_model, Model *f_parent);
    void RemoveLink(Model *f_model);

    void DoPulse_S1();
    void DoPulse_S2();

    friend class Core;
    friend class ElementManager;
    friend class InheritanceManager;
};

}
