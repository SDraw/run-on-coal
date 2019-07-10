#pragma once
#include "Interfaces/IElement.h"
#include "Utils/CustomArgument.h"

namespace ROC
{

class Element : public virtual IElement
{
    std::vector<Element*> m_children;
    std::vector<Element*> m_parents;

    std::unordered_map<std::string, CustomArgument> m_customDataMap;
    std::unordered_map<std::string, CustomArgument>::iterator m_customDataMapEnd;

    Element(const Element &that) = delete;
    Element& operator=(const Element &that) = delete;
public:
    bool HasChildren() const;
    const std::vector<Element*>& GetChildren() const { return m_children; }
    bool HasParents() const;
    const std::vector<Element*>& GetParents() const { return m_parents; }

    bool SetData(const std::string &f_key, const CustomArgument &f_val);
    bool GetData(const std::string &f_key, CustomArgument &f_val);
    bool RemoveData(const std::string &f_key);

    unsigned char GetElementType() const;
    const std::string& GetElementTypeName() const;
protected:
    unsigned char m_elementType;
    std::string m_elementTypeName;

    Element();
    virtual ~Element();

    // Derived classes use only
    void AddChild(Element *f_child);
    bool HasChild(Element *f_child);
    void RemoveChild(Element *f_child);

    void AddParent(Element *f_parent);
    bool HasParent(Element *f_parent);
    void RemoveParent(Element *f_parent);

    virtual void OnParentLinkDestroyed(Element *f_parent);
    virtual void OnChildLinkDestroyed(Element *f_child);

    friend class ElementManager;
};

}
