#include "stdafx.h"

namespace ROC
{

extern const glm::mat4 g_identityMatrix(1.f);
extern const glm::mat4 g_emptyMat4(0.f);
extern const glm::vec2 g_emptyVec2(0.f);
extern const glm::vec3 g_emptyVec3(0.f);
extern const glm::vec4 g_emptyVec4(0.f);
extern const glm::vec3 g_defaultScale(1.f, 1.f, 1.f);
extern const glm::quat g_defaultRotation(1.f, 0.f, 0.f, 0.f);
extern const float g_epsilon = glm::epsilon<float>();

extern const float g_quadVertexUV[]
{
    0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f, 1.f, 0.f
};
extern const float g_quadVertexNormals[]
{
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f
};

}
