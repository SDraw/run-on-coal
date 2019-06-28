#pragma once
#include "Interfaces/IGeometry.h"
#include "Elements/Element.h"

namespace ROC
{

class Material;
struct BoneCollisionData;
struct BoneData;
struct BoneJointData;

class Geometry final : public Element, public virtual IGeometry
{
    enum GeometrySetByte : unsigned char
    {
        GSB_Animated = 0x2U,
        GSB_Collision = 0xCBU
    };

    std::vector<Material*> m_materials;
    size_t m_materialCount;
    float m_boundSphereRaduis;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneCollisionData*> m_collisionData;
    std::vector<BoneJointData*> m_jointData;

    bool m_loaded;

    Geometry(const Geometry &that) = delete;
    Geometry& operator=(const Geometry &that) = delete;

    void Clear();
public:
    float GetBoundSphereRadius() const;
    size_t GetMaterialsCount() const;

    inline bool HasBonesData() const { return !m_bonesData.empty(); }
    inline bool HasBonesCollisionData() const { return !m_collisionData.empty(); }
    inline bool HasJointsData() const { return !m_jointData.empty(); }
protected:
    explicit Geometry();
    ~Geometry();

    bool Load(const std::string &f_path);
    void GenerateVAOs();

    inline const std::vector<Material*>& GetMaterials() const { return m_materials; }
    inline const std::vector<BoneData*>& GetBonesData() const { return m_bonesData; };
    inline const std::vector<BoneCollisionData*>& GetBonesCollisionData() const { return m_collisionData; }
    inline const std::vector<BoneJointData*>& GetJointsData() const { return m_jointData; }

    friend class ElementManager;
    friend class RenderManager;
    friend class AsyncGeometryTask;
    friend class Model;
};

}
