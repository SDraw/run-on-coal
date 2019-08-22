#pragma once

namespace ROC
{

class IAsyncManager
{
public:
    virtual void* LoadGeometry(const std::string &f_path) = 0;
    virtual void* LoadTexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress) = 0;
    virtual void* LoadTexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress) = 0;
};

}
