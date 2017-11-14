#pragma once

namespace GLUtils
{

inline bool IsSampler(GLenum f_type) { return ((f_type == GL_SAMPLER_2D || (f_type == GL_SAMPLER_2D_SHADOW)) || (f_type == GL_SAMPLER_CUBE)); }
inline bool Is2DSampler(GLenum f_type) { return ((f_type == GL_SAMPLER_2D || (f_type == GL_SAMPLER_2D_SHADOW))); }
inline bool IsCubicSampler(GLenum f_type) { return (f_type == GL_SAMPLER_CUBE); }

}

