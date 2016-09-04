#include "stdafx.h"
#include "Model/BoneChainData.h"
#include "Model/BoneChainGroup.h"

ROC::BoneChainGroup::BoneChainGroup()
{
}
ROC::BoneChainGroup::~BoneChainGroup()
{
    m_boneChainDataVector.clear();
}