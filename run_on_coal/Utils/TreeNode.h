#pragma once

namespace ROC
{

class Model;
class TreeNode
{
    Model *m_model;
    TreeNode *m_parent;
    std::vector<TreeNode*> m_children;
public:
    TreeNode(Model *f_model);
    ~TreeNode();

    inline Model* GetModel() { return m_model; }

    void AddChild(TreeNode *f_node);
    void RemoveChild(TreeNode *f_node);
    bool HasChild(TreeNode *f_node);
    inline std::vector<TreeNode*>& GetChildrenVectorRef() { return m_children; }

    inline void SetParent(TreeNode *f_node) { m_parent = f_node; }
    inline TreeNode* GetParent() { return m_parent; }
};

}
