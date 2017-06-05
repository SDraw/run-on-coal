#pragma once
#include "Elements/Element.h"
#include "Elements/Geometry/BoneCollisionData.hpp"
#include "Elements/Geometry/BoneData.hpp"
#include "Elements/Geometry/BoneJointData.hpp"

namespace ROC
{

class Material;
class Geometry : public Element
{
    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;

    float m_boundSphereRaduis;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneCollisionData*> m_collisionData;
    std::vector<BoneJointData*> m_jointData;

    enum gmLoadState : unsigned char { NotLoaded, Loading, Loaded };
    std::atomic<gmLoadState> m_loadState;
    bool m_async;

    void Clear();
public:
    inline bool IsLoaded() const { return (m_loadState == gmLoadState::Loaded); }
    inline float GetBoundSphereRadius() const { return m_boundSphereRaduis; }

    inline bool HasBonesData() const { return !m_bonesData.empty(); }
    inline bool HasBonesCollisionData() const { return !m_collisionData.empty(); }
    inline bool HasJointsData() const { return !m_jointData.empty(); }
protected:
    explicit Geometry(bool f_async);
    ~Geometry();
    bool Load(const std::string &f_path);
    void GenerateVAOs();

    inline bool IsAsyncLoad() const { return m_async; }

    inline std::vector<Material*>& GetMaterialVectorRef() { return m_materialVector; }
    inline std::vector<BoneData*>& GetBonesDataRef() { return m_bonesData; };
    inline std::vector<BoneCollisionData*>& GetBonesCollisionDataRef() { return m_collisionData; }
    inline std::vector<BoneJointData*>& GetJointsDataRef() { return m_jointData; };

    friend class AsyncManager;
    friend class ElementManager;
    friend class RenderManager;
    friend class Model;
};

}
