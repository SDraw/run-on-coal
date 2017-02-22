#pragma once

namespace ROC
{

class Element
{
public:
    enum ElementType : unsigned char { ClientElement = 0U, FileElement };
    inline unsigned char GetElementType() const { return m_elementType; }
protected:
    unsigned char m_elementType;

    Element();
    virtual ~Element();

    friend class ElementManager;
};

}
