#pragma once

namespace MathUtils
{

bool IsPowerOfTwo(int f_value);
float EaseInOut(float f_value);
void ExtractMatrix(const vr::HmdMatrix34_t &f_matVR, glm::mat4 &f_mat);
void ExtractMatrix(const vr::HmdMatrix44_t &f_matVR, glm::mat4 &f_mat);

}
