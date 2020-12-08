#pragma once
#include "Interfaces/IElement.h"

namespace ROC
{

class Element : public virtual IElement
{
    std::vector<Element*> m_children;
    std::vector<Element*> m_parents;

    Element(const Element &that) = delete;
    Element& operator=(const Element &that) = delete;
public:
    bool HasChildren() const;
    const std::vector<Element*>& GetChildren() const { return m_children; }
    bool HasParents() const;
    const std::vector<Element*>& GetParents() const { return m_parents; }

    unsigned char GetElementType() const;
    const std::string& GetElementTypeName() const;
protected:
    unsigned char m_elementType;

    Element();
    virtual ~Element();

    // Derived classes use only
    void AddChild(Element *f_child);
    bool HasChild(Element *f_child);
    void RemoveChild(Element *f_child);

    void AddParent(Element *f_parent);
    bool HasParent(Element *f_parent);
    void RemoveParent(Element *f_parent);

    // Must-call methods from derived classes
    virtual void OnParentRemoved(Element *f_parent);
    virtual void OnChildRemoved(Element *f_child);

    friend class ElementManager;
};

}
