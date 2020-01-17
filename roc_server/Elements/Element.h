#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class Element : public virtual IElement
{
    Element(const Element &that) = delete;
    Element& operator=(const Element &that) = delete;
public:
    unsigned char GetElementType() const;
    const std::string& GetElementTypeName() const;
protected:
    unsigned char m_elementType;
    std::string m_elementTypeName;

    Element();
    virtual ~Element();

    friend class ElementManager;
};

}
