#pragma once
#include "Elements/Element.h"

namespace ROC
{

class Material;
struct BoneCollisionData;
struct BoneData;
struct BoneJointData;

class Geometry final : public Element
{
    enum GeometrySetByte : unsigned char
    {
        GSB_Animated = 0x2U,
        GSB_Collision = 0xCBU
    };

    std::vector<Material*> m_materialVector;
    unsigned int m_materialCount;

    float m_boundSphereRaduis;

    std::vector<BoneData*> m_bonesData;
    std::vector<BoneCollisionData*> m_collisionData;
    std::vector<BoneJointData*> m_jointData;

    enum GeometryLoadState : unsigned char
    {
        GLS_NotLoaded,
        GLS_Loading,
        GLS_Loaded
    };
    std::atomic<GeometryLoadState> m_loadState;
    bool m_async;
    bool m_released;

    Geometry(const Geometry &that);
    Geometry& operator=(const Geometry &that);

    void Clear();
public:
    inline bool IsLoaded() const { return (m_loadState == GLS_Loaded); }
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
    inline bool IsReleased() const { return m_released; }

    inline const std::vector<Material*>& GetMaterialVector() const { return m_materialVector; }
    inline const std::vector<BoneData*>& GetBonesData() const { return m_bonesData; };
    inline const std::vector<BoneCollisionData*>& GetBonesCollisionData() const { return m_collisionData; }
    inline const std::vector<BoneJointData*>& GetJointsData() const { return m_jointData; }

    friend class AsyncManager;
    friend class ElementManager;
    friend class RenderManager;
    friend class Model;
};

}
