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

}
