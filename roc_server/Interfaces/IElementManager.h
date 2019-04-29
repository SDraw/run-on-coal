#pragma once

namespace ROC
{

class IElement;
class IFile;

class IElementManager
{
public:
    virtual IFile* CreateFile_(const std::string &f_path) = 0;
    virtual IFile* OpenFile(const std::string &f_path, bool f_ro) = 0;;

    virtual bool IsValidElement(IElement *f_ptr) = 0;
    virtual bool DestroyElement(IElement *f_element) = 0;
};

}