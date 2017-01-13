#include "stdafx.h"
#include "Utils/Pool.h"

ROC::Pool::Pool(unsigned int f_size)
{
    m_size = f_size;
    m_poolData.assign(m_size, 0U);
    m_minimal = 0U;
}
ROC::Pool::~Pool()
{
    m_poolData.clear();
}

int ROC::Pool::Allocate()
{
    int l_allocated = -1;
    for(unsigned int i = m_minimal; i < m_size; i++)
    {
        if(m_poolData[i] == 0U)
        {
            m_poolData[i] = 1U;
            l_allocated = static_cast<int>(i);
            break;
        }
    }
    if(l_allocated == -1)
    {
        for(unsigned int i = 0; i < m_minimal; i++)
        {
            if(m_poolData[i] == 0U)
            {
                m_poolData[i] = 1U;
                l_allocated = static_cast<int>(i);
                break;
            }
        }
    }
    if(l_allocated != -1)
    {
        for(unsigned int i = static_cast<unsigned int>(l_allocated + 1); i < m_size; i++)
        {
            if(m_poolData[i] == 0U)
            {
                m_minimal = i;
                break;
            }
        }
    }
    return l_allocated;
}
void ROC::Pool::Reset(unsigned int f_id)
{
    if(f_id < m_size)
    {
        m_poolData[f_id] = 0U;
        if(f_id < m_minimal) m_minimal = f_id;
    }
}
