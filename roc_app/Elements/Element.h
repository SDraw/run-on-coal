#pragma once
#include "Interfaces/IElement.h"
#include "Utils/CustomArgument.h"

namespace ROC
{

class Element : public virtual IElement
{
    std::unordered_map<std::string, CustomArgument> m_customDataMap;
    std::unordered_map<std::string, CustomArgument>::iterator m_customDataMapEnd;

    Element(const Element &that) = delete;
    Element& operator=(const Element &that) = delete;
public:
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

    friend class ElementManager;
};

}
