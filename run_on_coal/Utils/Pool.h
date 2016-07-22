#pragma once

namespace ROC
{

class Pool
{
    unsigned int m_size;
    std::vector<unsigned char> m_poolData;
    unsigned int m_minimal;
public:
    Pool(unsigned int f_size);
    ~Pool();
    int Allocate();
    void Reset(unsigned int f_id);
};

}
