#include "stdafx.h"
#include "Utils/Event.h"

ROC::Event::Event(void *f_func, int f_ref)
{
    m_luaFunc = f_func;
    m_luaRef = f_ref;
    m_deleted = false;
    m_muted = false;
}
ROC::Event::~Event()
{
}
