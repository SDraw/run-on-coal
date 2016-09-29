#pragma once

namespace ROC
{
enum EventType : unsigned char
{
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
class Event;
class EventManager
{
    LuaManager *m_luaManager;
    std::vector<Event*> m_eventsVector[EventType::Last];
    std::vector<Event*>::iterator m_iter;
    unsigned char m_activeEvent;
public:
    void CallEvent(unsigned char f_event, LuaArguments *f_args);
    bool AddEvent(unsigned char f_event, int f_ref, void *f_pointer);
    bool SetEventMute(unsigned char f_event, void *f_pointer, bool f_mute);
    bool RemoveEvent(unsigned char f_event, void *f_pointer);
protected:
    EventManager(LuaManager *f_luaManager);
    ~EventManager();
    friend LuaManager;
};
}
