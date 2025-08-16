#pragma once

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

typedef struct _Point
{
    i32 x, y;
} Point;

void Input_UpdatePrevState();

void Input_SetKeyState(u32 key, bool pressed);
void Input_SetMouseButton(u32 key, bool pressed);
void Input_SetMousePos(i32 x, i32 y);

bool Input_IsKeyPressed(u32 key);
bool Input_IsKeyReleased(u32 key);
bool Input_IsKeyHeld(u32 key);

bool Input_IsMouseClicked(u32 button);
bool Input_IsMouseReleased(u32 button);
bool Input_IsMouseHeld(u32 button);
