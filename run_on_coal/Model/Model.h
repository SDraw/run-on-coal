#pragma once
#define MODEL_RIGIDITY_TYPE_SPHERE 0U
#define MODEL_RIGIDITY_TYPE_BOX 1U
#define MODEL_RIGIDITY_TYPE_CYLINDER 2U
#define MODEL_RIGIDITY_TYPE_CAPSULE 3U
#define MODEL_RIGIDITY_TYPE_CONE 4U
#define MODEL_TYPE_NONE 0
#define MODEL_TYPE_STATIC 1
#define MODEL_TYPE_ANIMATED 2

namespace ROC
{

class Geometry;
class Skeleton;
class Animation; 
class Model
{
    Geometry *m_geometry; 
    Skeleton *m_skeleton;
    Animation *m_animation;
    Model *m_parent;
    btRigidBody* m_rigidBody;
        
    int m_parentBone;
    unsigned long m_animLastTick;
    unsigned long m_animCurrentTick;
    enum AnimationState { None = 0U, Paused, Playing };
    AnimationState m_animState;
    float m_animationSpeed;

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_localMatrix;
    glm::mat4 m_matrix;
    bool m_rebuildMatrix;

    void UpdateSkeleton();
    void UpdateAnimationTick();
public:
    bool IsDrawable();
    int GetType();
    bool IsRigid();
    //Manipulation
    void SetPosition(glm::vec3 &f_pos, bool f_uRb = true);
    void GetPosition(glm::vec3 &f_pos, bool f_global = false);
    void SetRotation(glm::quat &f_rot, bool f_uRb = true);
    void GetRotation(glm::quat &f_rot, bool f_global = false);
    void SetScale(glm::vec3 &f_scl);
    void GetScale(glm::vec3 &f_scl, bool f_global = false);

    void GetMatrix(glm::mat4 &f_mat);

    unsigned int GetMaterialCount();
    unsigned char GetMaterialType(unsigned int f_material);
    void GetMaterialParam(unsigned int f_material, glm::vec4 &f_vec);

    bool PlayAnimation();
    bool PauseAnimation();
    bool ResetAnimation();
    bool SetAnimationSpeed(float f_val);
    float GetAnimationSpeed();
    bool SetAnimationProgress(float f_val);
    float GetAnimationProgress();
    bool SetBonePosition(unsigned int f_bone, glm::vec3 &f_val);
    bool SetBoneRotation(unsigned int f_bone, glm::quat &f_val);
    bool SetBoneScale(unsigned int f_bone, glm::vec3 &f_val);

    bool HasSkeleton();
    bool HasRigidSkeleton();
    int GetBonesCount();
    void GetBoneMatrices(std::vector<glm::mat4> &f_mat);
    bool GetBoneMatrix(unsigned int f_bone,glm::mat4 &f_mat);

    //Physics
    bool SetVelocity(glm::vec3 &f_val);
    bool GetVelocity(glm::vec3 &f_val);
    bool SetAngularVelocity(glm::vec3 &f_val);
    bool GetAngularVelocity(glm::vec3 &f_val);
    float GetMass();
    bool SetFriction(float f_val);
    float GetFriction();

    Geometry* GetGeometry();
    Model* GetParent();
    Animation* GetAnimation();
protected:
    Model(Geometry *f_geometry);
    ~Model();
    void DrawMaterial(unsigned int f_material, bool f_texturize, bool f_binding);
    void SetParent(Model *f_model, int f_bone = -1);
    void SetAnimation(Animation *f_anim);
    void SetGeometry(Geometry *f_geometry);
    void UpdateMatrix();
    void UpdateAnimation();
    GLuint GetMaterialVAO(unsigned int f_material);
    GLuint GetMaterialTexture(unsigned int f_material);
    bool SetRigidity(unsigned char f_type, float f_mass, glm::vec3 &f_dim);
    bool RemoveRigidity();
    btRigidBody* GetRidigBody();
    void GetSkeletonRigidData(std::vector<btRigidBody*> &f_rb, std::vector<btTypedConstraint*> &f_cs);
    void UpdateSkeletonChains();
    void UpdateSkeletonRigidBones();
    void UpdateRigidity();

    friend class ElementManager;
    friend class InheritanceManager;
    friend class PhysicsManager;
    friend class PreRenderManager;
    friend class RenderManager;
};

}
