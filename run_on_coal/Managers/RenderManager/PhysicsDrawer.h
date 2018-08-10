#pragma once

namespace ROC
{

class PhysicsDrawer final : public btIDebugDraw
{
    GLuint m_VBO;
    GLuint m_VAO;

    std::vector<glm::vec3> m_lines;
public:
    PhysicsDrawer();
    ~PhysicsDrawer();

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {};
    virtual void reportErrorWarning(const char* warningString) {};
    virtual void draw3dText(const btVector3& location, const char* textString) {};
    virtual void setDebugMode(int debugMode) {};
    virtual int	getDebugMode() const { return btIDebugDraw::DBG_DrawWireframe; };
protected:
    void Draw();

    friend class RenderManager;
};

}
