#include "WindowsAPI/KeyboardComponent.h"

#include <iostream>


std::unordered_map<int, bool> KeyboardComponent::mKeyPressed{};
std::unordered_map<int, bool> KeyboardComponent::mSinglePressed{};

void KeyboardComponent::HandleWindowMessage(UINT message, WPARAM wParam) noexcept
{
    int key = static_cast<int>(wParam);
    if (key < 0 || key > 255)
    {
        return;
    }

    switch (message)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        mKeyPressed[key] = true;
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        mKeyPressed[key] = false;
        mSinglePressed[key] ^= true;
        break;
    }
}

bool KeyboardComponent::IsKeyPressed(int key)
{
    auto it = mKeyPressed.find(key);
    if (it != mKeyPressed.end())
    {
        return it->second;
    }
    return false;
}

bool KeyboardComponent::IsActive(int key)
{
    auto it = mSinglePressed.find(key);
    if (it != mSinglePressed.end())
    {
        return it->second;
    }
    return false;
}
