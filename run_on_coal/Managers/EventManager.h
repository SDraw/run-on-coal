#pragma once

namespace ROC
{

enum EventType : unsigned char {
    PreRender = 0U, //onOGLPreRender
    Render, //onOGLRender
    WindowResize, //onWindowResize
    KeyPress, //onKeyPress
    MouseKeyPress, //onMouseKeyPress
    MouseScroll, //onMouseScroll
    CursorMove, //onCursorMove
    CursorEnter, //onCursorEnter
    JoypadConnect, //onJoypadConnect
    JoypadButton, //onJoypadButton
    JoypadAxis, //onJoypadAxis
    TextInput, //onTextInput
    Last // None
};

class LuaManager;
class LuaArguments;
class EventManager
{
    LuaManager *m_luaManager;

    std::vector<std::pair<void*,int>> m_eventVector[EventType::Last];
    size_t m_lastIteration;
    unsigned char m_currentEvent;
public:
    void CallEvent(unsigned char f_event,LuaArguments &f_args);
    bool IsEventExists(unsigned char f_event);
    bool AddEvent(unsigned char f_event, int f_ref, void *f_pointer);
    bool RemoveEvent(unsigned char f_event, void *f_pointer);
protected:
    EventManager(LuaManager *f_luaManager);
    ~EventManager();
    friend LuaManager;
};

}
