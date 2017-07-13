#pragma once

namespace ROC
{

class TreeNode
{
    void *m_ptr;
    TreeNode *m_parent;
    std::vector<TreeNode*> m_children;
public:
    explicit TreeNode(void *f_ptr);
    ~TreeNode();

    inline void* GetPointer() { return m_ptr; }

    void AddChild(TreeNode *f_node);
    void RemoveChild(TreeNode *f_node);
    bool HasChild(TreeNode *f_node);
    inline const std::vector<TreeNode*>& GetChildrenVectorRef() const { return m_children; }

    inline void SetParent(TreeNode *f_node) { m_parent = f_node; }
    inline TreeNode* GetParent() { return m_parent; }
};

}
