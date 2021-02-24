
#include "Event.hpp"

Event::Event()
{

}
void Event::initKeyPress(const unsigned char& key, const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    m_key = convertKeyId(key);
    m_keyid = key;
    m_isKeyboardEvent = true;
    m_isKeyPressedEvent = true;
    m_isKeyReleasedEvent = false;
}
void Event::initKeyReleased(const unsigned char& key, const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    m_key = convertKeyId(key);
    m_isKeyboardEvent = true;
    m_isKeyPressedEvent = false;
    m_isKeyReleasedEvent = true;
}

void Event::initSpecialKeyPress(const int& key, const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    m_key = convertspecialKeyId(key);
    m_isKeyboardEvent = true;
    m_isKeyPressedEvent = true;
    m_isKeyReleasedEvent = false;
}
void Event::initSpecialKeyReleased(const int& key, const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    m_key = convertspecialKeyId(key);
    m_isKeyboardEvent = true;
    m_isKeyPressedEvent = false;
    m_isKeyReleasedEvent = true;
}

void Event::initMouseButton(const int& button,  const int& state, const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    if (button >= 0 && button <= 2)
    {
        m_button = (MouseButton) button;
        m_isMouseEvent = true;
        m_isMouseButtonPressedEvent = state == GLFW_PRESS;
        m_isMouseButtonReleasedEvent = state == GLFW_RELEASE;
        m_isMouseMoveEvent = false;
        m_isMousewheelEvent = false;

        if (m_isMouseButtonPressedEvent)
        {
           switch (m_button)
           {

            case MouseButton::Wheel:
                Event::m_middlePressed = true;
                break;
            case MouseButton::Left:
                Event::m_leftPressed = true;
                break;
            case MouseButton::Right:
                Event::m_rightPressed = true;
                break;
            default:
                break;
           }
        }
        else
        {
            switch (m_button)
           {
            case MouseButton::Left:
                Event::m_leftPressed = false;
                break;
            case MouseButton::Right:
                Event::m_rightPressed = false;
                break;
            case MouseButton::Wheel:
                Event::m_middlePressed = false;
                break;
            default:
                break;
           }
        }

        
    }
    else if (button == 3)
    {
        if (state == 0)
        {
            m_button = MouseButton::Wheel;
            m_isMouseEvent = true;
            m_isMouseButtonPressedEvent = false;
            m_isMouseButtonReleasedEvent = false;
            m_isMouseMoveEvent = false;
            m_isMousewheelEvent = true;
            m_weelDelta = -1;
        }
    }
    else if (button == 4)
    {
        if (state == 0)
        {
            m_button = MouseButton::Wheel;
            m_isMouseEvent = true;
            m_isMouseButtonPressedEvent = false;
            m_isMouseButtonReleasedEvent = false;
            m_isMouseMoveEvent = false;
            m_isMousewheelEvent = true;
            m_weelDelta = 1;
        }
    }
}

void Event::initMouseMove(const int& x, const int& y)
{
    m_xmouse = x;
    m_ymouse = y;
    m_isMouseEvent = true;
    m_isMouseButtonPressedEvent = false;
    m_isMouseButtonReleasedEvent = false;
    m_isMouseMoveEvent = true;
    m_isMousewheelEvent = false;
}



bool Event::isKeyboardEvent() const
{
    return m_isKeyboardEvent;
}
bool Event::isKeyPressedEvent() const
{
    return m_isKeyPressedEvent;
}
bool Event::isKeyReleasedEvent() const
{
    return m_isKeyReleasedEvent;
}

bool Event::isMouseEvent() const
{
    return m_isMouseEvent;
}
bool Event::isMouseButtonPressedEvent() const
{
    return m_isMouseButtonPressedEvent;
}
bool Event::isMouseButtonReleasedEvent() const
{
    return m_isMouseButtonReleasedEvent;
}
bool Event::isMouseMoveEvent() const
{
    return m_isMouseMoveEvent;
}
bool Event::isMousewheelEvent() const
{
    return m_isMousewheelEvent;
}

int Event::getMouseX() const
{
    return m_xmouse;
}
int Event::getMouseY() const
{
    return m_ymouse;
}

int Event::getMouseWeelDelta() const
{
    return m_weelDelta;
}


Key Event::getKey() const
{
    return m_key;
}

MouseButton Event::getMouseButton() const
{
    return m_button;
}



///////////////////////////////////////////////////////////////
// méthodes statiques
///////////////////////////////////////////////////////////////


Key Event::convertKeyId(const unsigned char& key)
{

//    std::cerr <<"c:" <<(int)key<<std::endl;
    Key r;
    if (key >= 'A' && key <= 'Z')
    {
        r = (Key)((int) Key::A + ((int)key - (int)'A'));
    }
    else if(key >='a' && key <='z')
    {
        r = (Key)((int) Key::A + ((int)key - (int)'a'));
    }
    else if(key >='0' && key <='9')
    {
        r = (Key)((int) Key::Numpad0 + ((int)key - (int)'0'));
    }
    else if (key == 27)
    {
        r = Key::Escape;
    }
    else if (key == 32)
    {
        r = Key::Space;
    }
    else if (key == 8)
    {
        r = Key::BackSpace;
    }
    else if (key == 127)
    {
        r = Key::Delete;
    }
    else if (key == 61)
    {
        r = Key::Equal;
    }
    else if (key == 9)
    {
        r = Key::Tab;
    }
    else if (key == 13)
    {
        r = Key::Return;
    }
    else if (key == 224)
    {
        r = Key::Num0;
    }
    else if (key == 38)
    {
        r = Key::Num1;
    }
    else if (key == 233)
    {
        r = Key::Num2;
    }
    else if (key == 34)
    {
        r = Key::Num3;
    }
    else if (key == 39)
    {
        r = Key::Num4;
    }
    else if (key == 40)
    {
        r = Key::Num5;
    }
    // else if (key == 45)
    // {
    //     r = Key::Num6;
    // }
    else if (key == 232)
    {
        r = Key::Num7;
    }
    else if (key == 95)
    {
        r = Key::Num8;
    }
    else if (key == 231)
    {
        r = Key::Num9;
    }
    else if (key == 43)
    {
        r = Key::Add;
    }
    else if (key == 45)
    {
        r = Key::Subtract;
    }
    else if (key == 42)
    {
        r = Key::Multiply;
    }
    else if (key == 47)
    {
        r = Key::Divide;
    }
    else
    {
        r = Key::Unknown;
    }

    return r;
}
Key Event::convertspecialKeyId(const int& key)
{
    //std::cerr << key <<std::endl;
    Key r;
    if (key == 100)
    {
        r = Key::Left;
    }
    else if (key == 101)
    {
        r = Key::Up;
    }
    else if (key == 102)
    {
        r = Key::Right;
    }
    else if (key == 103)
    {
        r = Key::Down;
    }
    else if (key == 112)
    {
        r = Key::LShift;
    }
    else if (key == 113)
    {
        r = Key::RShift;
    }
    else if (key == 114)
    {
        r = Key::LControl;
    }
    else if (key == 115)
    {
        r = Key::RControl;
    }
    else if (key == 116)
    {
        r = Key::LAlt;
    }
    else if (key == 117)
    {
        r = Key::RAlt;
    }
    else if (key == 1)
    {
        r = Key::F1;
    }
    else if (key == 2)
    {
        r = Key::F2;
    }
    else if (key == 3)
    {
        r = Key::F3;
    }
    else if (key == 4)
    {
        r = Key::F4;
    }
    else if (key == 5)
    {
        r = Key::F5;
    }
    else if (key == 6)
    {
        r = Key::F6;
    }
    else if (key == 7)
    {
        r = Key::F7;
    }
    else if (key == 8)
    {
        r = Key::F8;
    }
    else if (key == 9)
    {
        r = Key::F9;
    }
    else if (key == 10)
    {
        r = Key::F10;
    }
    else if (key == 11)
    {
        r = Key::F11;
    }
    else if (key == 12)
    {
        r = Key::F12;
    }
    else
    {
        r = Key::Unknown;
    }
    return r;
}

///////////////////////////////////////////////////////////////
// init valiables statiques
///////////////////////////////////////////////////////////////
bool Event::m_leftPressed = false;
bool Event::m_rightPressed = false;
bool Event::m_middlePressed = false;

///////////////////////////////////////////////////////////////
// implementation des méthodes statiques d'évenement
///////////////////////////////////////////////////////////////
bool Event::MouseLeftPressed()
{
    return Event::m_leftPressed;
}
bool Event::MouseRightPressed()
{
    return Event::m_rightPressed;
}
bool Event::MouseWheelPressed()
{
    return Event::m_middlePressed;
}