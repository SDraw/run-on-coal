#pragma once
#include "Utils/CustomData.h"

namespace ROC
{

class Element
{
    std::unordered_map<std::string, CustomData> m_customDataMap;
    std::unordered_map<std::string, CustomData>::iterator m_customDataMapEnd;

    Element(const Element &that);
    Element& operator=(const Element &that);
public:
    enum ElementType : unsigned char
    {
        ET_Geometry = 0U, 
        ET_Model, 
        ET_Animation, 
        ET_Scene,
        ET_Camera, 
        ET_Light, 
        ET_RenderTarget, 
        ET_Shader,
        ET_Sound, 
        ET_Texture, 
        ET_Font, 
        ET_File, 
        ET_Collision
    };

    bool SetCustomData(const std::string &f_key, CustomData &f_val);
    void GetCustomData(const std::string &f_key, CustomData &f_val);
    bool RemoveCustomData(const std::string &f_key);

    inline unsigned char GetElementType() const { return m_elementType; }
    inline const std::string& GetElementTypeName() const { return m_elementTypeName; }
protected:
    unsigned char m_elementType;
    std::string m_elementTypeName;

    Element();
    virtual ~Element();

    friend class ElementManager;
};

}
