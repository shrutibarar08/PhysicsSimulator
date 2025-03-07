#pragma once

#include <windows.h>
#include <unordered_map>

/*
 *	Manages Windows keyboard action.
 */
class KeyboardComponent
{
public:

	static void HandleWindowMessage(UINT message, WPARAM wParam) noexcept;
	static bool IsKeyPressed(int key);
	static bool IsActive(int key);

private:
	static std::unordered_map<int, bool> mKeyPressed;
	static std::unordered_map<int, bool> mSinglePressed;
};
