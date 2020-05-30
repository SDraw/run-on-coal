#include "stdafx.h"

namespace ROC
{

extern const glm::mat4 g_IdentityMatrix(1.f);
extern const glm::mat4 g_EmptyMat4(0.f);
extern const glm::vec2 g_EmptyVec2(0.f);
extern const glm::vec3 g_EmptyVec3(0.f);
extern const glm::vec4 g_EmptyVec4(0.f);
extern const glm::vec3 g_DefaultScale(1.f, 1.f, 1.f);
extern const glm::quat g_DefaultRotation(1.f, 0.f, 0.f, 0.f);
extern const float g_Epsilon = glm::epsilon<float>();

extern const float g_QuadVertexUV[] = {
    0.f, 0.f, 0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f, 1.f, 0.f
};
extern const float g_QuadVertexNormals[] = {
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f,
    0.f, 0.f, -1.f, 0.f, 0.f, -1.f, 0.f, 0.f, -1.f
};

}
