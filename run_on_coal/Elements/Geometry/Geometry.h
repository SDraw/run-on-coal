#pragma once

namespace ROC
{

class BoneData;
class BoneCollisionData;
class BoneJointData;
class Material;
class Geometry
{
    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneCollisionData*> m_collisionData;
    std::vector<BoneJointData*> m_jointData;

    bool m_loaded;
    void Clear();
public:
    inline bool HasBonesData() const { return !m_bonesData.empty(); }
    inline bool HasBonesCollisionData() const { return !m_collisionData.empty(); }
    inline bool HasJointsData() const { return !m_jointData.empty(); }
protected:
    Geometry();
    ~Geometry();
    inline std::vector<Material*>& GetMaterialVectorRef() { return m_materialVector; }
    bool Load(std::string &f_path);

    inline std::vector<BoneData*> &GetBonesDataRef() { return m_bonesData; };
    inline std::vector<BoneCollisionData*> &GetBonesCollisionDataRef() { return m_collisionData; }
    inline std::vector<BoneJointData*> &GetJointsDataRef() { return m_jointData; };

    friend class ElementManager;
    friend class RenderManager;
    friend class Model;
    friend class Skeleton;
};

}
