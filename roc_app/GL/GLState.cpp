#include "stdafx.h"

#include "GL/GLState.h"

void GLState::Finish()
{
    glFinish();
}

void GLState::Flush()
{
    glFlush();
}
