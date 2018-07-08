#include "stdafx.h"

#include "Utils/MathUtils.h"

namespace MathUtils
{

bool IsPowerOfTwo(int f_value)
{
    return (f_value > 0 && ((f_value & (f_value - 1)) == 0));
}
float EaseInOut(float f_value)
{
    return -0.5f*(cos(glm::pi<float>()*f_value) - 1.f);
}
void ExtractMatrix(const vr::HmdMatrix34_t &f_matVR, glm::mat4 &f_mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 3; j++) f_mat[i][j] = f_matVR.m[j][i];
    }
    for(int i = 0; i < 3; i++) f_mat[i][3] = 0.f;
    f_mat[3][3] = 1.f;
}
void ExtractMatrix(const vr::HmdMatrix44_t &f_matVR, glm::mat4 &f_mat)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            f_mat[i][j] = f_matVR.m[j][i];
        }
    }
}

}
