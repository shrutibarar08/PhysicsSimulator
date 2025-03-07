#include "WindowsAPI/MouseComponent.h"

#include <iostream>

#include "Core/SystemManager/SubsystemManager.h"
#include "WindowsAPI/WindowsManager.h"

int MouseComponent::mLastMouseX = 0;
int MouseComponent::mLastMouseY = 0;
float MouseComponent::mDeltaX = 0.0f;
float MouseComponent::mDeltaY = 0.0f;
bool MouseComponent::mFirstMouse = true;
HWND MouseComponent::mWindowHandle = nullptr;
bool MouseComponent::mFreelook = false;

void MouseComponent::HandleWindowMessage(UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (!mWindowHandle || !mFreelook) return;

    switch (message)
    {
    case WM_MOUSEMOVE:
    {
        // Get current cursor position
        POINT mousePos;
        GetCursorPos(&mousePos);

        int centerX = 400;
        int centerY = 300;

        if (mFirstMouse)
        {
            mLastMouseX = centerX;
            mLastMouseY = centerY;
            mFirstMouse = false;
            SetCursorPos(centerX, centerY); // Ensure first position is centered
            return;
        }

        // Ignore movement if cursor was just reset
        if (mousePos.x == centerX && mousePos.y == centerY)
        {
            return;
        }

        // Compute delta movement
        mDeltaX = static_cast<float>(mousePos.x - mLastMouseX);
        mDeltaY = static_cast<float>(mousePos.y - mLastMouseY);

        // Reset cursor to center AFTER computing movement
        SetCursorPos(centerX, centerY);

        // Immediately set last position to avoid false movement on next frame
        mLastMouseX = centerX;
        mLastMouseY = centerY;
        break;
    }
    }
}

void MouseComponent::SetWindowHandle(HWND hWnd)
{
    mWindowHandle = hWnd;
}

void MouseComponent::ToggleFreelock(bool status)
{
    if (status && !mFreelook)
    {
        ResetMouseDelta();
        mFreelook = true;
        SubsystemManager::Get<WindowsManager>()->EnableFreelook();
        std::cout << "Free lock activated!\n";
    }
    if (!status && mFreelook)
    {
        mFreelook = false;
        SubsystemManager::Get<WindowsManager>()->DisableFreelook();
        std::cout << "Free lock deactivated!\n";
    }
}

float MouseComponent::GetDeltaX() { return mDeltaX; }
float MouseComponent::GetDeltaY() { return mDeltaY; }
void MouseComponent::ResetMouseDelta() { mDeltaX = 0; mDeltaY = 0; }
