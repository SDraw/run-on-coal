#pragma once

namespace ROC
{

class IElement;
class IFile;

class IElementManager
{
public:
    virtual IFile* CreateIFile(const std::string &f_path) = 0;
    virtual IFile* OpenIFile(const std::string &f_path, bool f_ro) = 0;

    virtual bool IsValidIElement(IElement *f_ptr) = 0;
    virtual bool DestroyIElement(IElement *f_element) = 0;
};

}
