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

    bool HasBonesData() const;
    bool HasBonesCollisionData() const;
    bool HasJointsData() const;
protected:
    explicit Geometry();
    ~Geometry();

    bool Load(const std::string &f_path);
    void GenerateMaterials();

    const std::vector<Material*>& GetMaterials() const;
    const std::vector<BoneData*>& GetBonesData() const;
    const std::vector<BoneCollisionData*>& GetBonesCollisionData() const;
    const std::vector<BoneJointData*>& GetJointsData() const;

    friend class ElementManager;
    friend class RenderManager;
    friend class AsyncGeometryTask;
    friend class Model;
};

}
