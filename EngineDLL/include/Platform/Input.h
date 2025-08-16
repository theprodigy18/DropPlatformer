#pragma once

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1

typedef struct _Point
{
    i32 x, y;
} Point;

void DROP_UpdatePrevState();

void DROP_SetKeyState(u32 key, bool pressed);
void DROP_SetMouseButton(u32 key, bool pressed);
void DROP_SetMousePos(i32 x, i32 y);

bool DROP_IsKeyPressed(u32 key);
bool DROP_IsKeyReleased(u32 key);
bool DROP_IsKeyHeld(u32 key);

bool DROP_IsMouseClicked(u32 button);
bool DROP_IsMouseReleased(u32 button);
bool DROP_IsMouseHeld(u32 button);
