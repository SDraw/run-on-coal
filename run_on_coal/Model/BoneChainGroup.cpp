#include "stdafx.h"
#include "Model/BoneChainGroup.h"

ROC::BoneChainGroup::BoneChainGroup()
{
}
ROC::BoneChainGroup::~BoneChainGroup()
{
    for(auto iter : m_boneChainDataVector) delete iter;
    m_boneChainDataVector.clear();
}

void ROC::BoneChainGroup::AddChain(unsigned char f_type, float f_mass, glm::vec3 &f_size, int f_boneID)
{
    BoneChainData *l_chain = new BoneChainData();
    l_chain->m_type = f_type;
    l_chain->m_mass = f_mass;
    std::memcpy(&l_chain->m_size, &f_size, sizeof(glm::vec3));
    l_chain->m_boneID = f_boneID;
    m_boneChainDataVector.push_back(l_chain);
}