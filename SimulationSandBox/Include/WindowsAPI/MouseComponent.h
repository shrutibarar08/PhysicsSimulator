#pragma once
#include <Windows.h>


class MouseComponent
{
public:
    static void HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam) noexcept;
    static float GetDeltaX();
    static float GetDeltaY();
    static void ResetMouseDelta();
    static void SetWindowHandle(HWND hWnd);
    static void ToggleFreelock(bool status);

private:
    static int mLastMouseX;
    static int mLastMouseY;
    static float mDeltaX;
    static float mDeltaY;
    static bool mFirstMouse;
    static HWND mWindowHandle;
	static bool mFreelook;
};
