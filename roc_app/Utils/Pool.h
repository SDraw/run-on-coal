#pragma once

namespace ROC
{

class Pool
{
    size_t m_size;
    std::vector<unsigned char> m_poolData;
    size_t m_minimal;
public:
    explicit Pool(size_t f_size);
    ~Pool();

    int Allocate();
    void Reset(size_t f_id);
};

}
