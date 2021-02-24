#pragma once

#include <GLFW/glfw3.h>

///////////////////////////////////////////////////////////////
// énumération des boutons de souris

enum class MouseButton
{
    Unknown = -1, ///< Unhandled key
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Wheel = GLFW_MOUSE_BUTTON_MIDDLE,
    Right = GLFW_MOUSE_BUTTON_RIGHT
};