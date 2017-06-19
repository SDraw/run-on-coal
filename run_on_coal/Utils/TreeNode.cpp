#include "stdafx.h"
#include "Utils/TreeNode.h"

ROC::TreeNode::TreeNode(void *f_model)
{
    m_pointer = f_model;
    m_parent = nullptr;
}
ROC::TreeNode::~TreeNode()
{
    m_children.clear();
}

void ROC::TreeNode::AddChild(TreeNode *f_node)
{
    auto l_end = m_children.end();
    if(std::find(m_children.begin(), l_end, f_node) == l_end) m_children.push_back(f_node);
}
void ROC::TreeNode::RemoveChild(TreeNode *f_node)
{
    auto iter = std::find(m_children.begin(), m_children.end(), f_node);
    if(iter != m_children.end()) m_children.erase(iter);
}
bool ROC::TreeNode::HasChild(TreeNode *f_node)
{
    auto l_end = m_children.end();
    return (std::find(m_children.begin(), l_end, f_node) != l_end);
}
