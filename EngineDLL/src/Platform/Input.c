#include "pch.h"
#include "Platform/Input.h"

#pragma region INTERNAL
#define KEY_COUNT 256
#define BUTTON_COUNT 2

typedef struct _InputState
{
    bool keys[KEY_COUNT];
    bool prevKeys[KEY_COUNT];
} InputState;

typedef struct _MouseState
{
    bool buttons[BUTTON_COUNT];
    bool prevButtons[BUTTON_COUNT];
} MouseState;

typedef struct _MousePos
{
    Point pos;
    Point prevPos;
} MousePos;

static InputState s_inputState;
static MouseState s_mouseState;
static MousePos   s_mousePos;
#pragma endregion

void DROP_UpdatePrevState()
{
    for (u32 i = 0; i < KEY_COUNT; ++i)
    {
        s_inputState.prevKeys[i] = s_inputState.keys[i];
    }

    s_mouseState.prevButtons[MOUSE_BUTTON_LEFT]  = s_mouseState.buttons[MOUSE_BUTTON_LEFT];
    s_mouseState.prevButtons[MOUSE_BUTTON_RIGHT] = s_mouseState.buttons[MOUSE_BUTTON_RIGHT];

    s_mousePos.prevPos = s_mousePos.pos;
}

void DROP_SetKeyState(u32 key, bool pressed)
{
    s_inputState.keys[key] = pressed;
}

void DROP_SetMouseButton(u32 key, bool pressed)
{
    s_mouseState.buttons[key] = pressed;
}

void DROP_SetMousePos(i32 x, i32 y)
{
    s_mousePos.pos.x = x;
    s_mousePos.pos.y = y;
}

bool DROP_IsKeyPressed(u32 key)
{
    return s_inputState.keys[key] && !s_inputState.prevKeys[key];
}

bool DROP_IsKeyReleased(u32 key)
{
    return !s_inputState.keys[key] && s_inputState.prevKeys[key];
}

bool DROP_IsKeyHeld(u32 key)
{
    return s_inputState.keys[key];
}

bool DROP_IsMouseClicked(u32 button)
{
    return s_mouseState.buttons[button] && !s_mouseState.prevButtons[button];
}

bool DROP_IsMouseReleased(u32 button)
{
    return !s_mouseState.buttons[button] && s_mouseState.prevButtons[button];
}

bool DROP_IsMouseHeld(u32 button)
{
    return s_mouseState.buttons[button];
}