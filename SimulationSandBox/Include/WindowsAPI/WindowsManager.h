#pragma once

#include <memory>

#include "Core/Interface/ISubsystemInterface.h"

#include <Windows.h>
#include <string>
#include <optional>

#include "imgui/imgui_impl_win32.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class WindowsManager final: public ISubsystemInterface
{
public:
	WindowsManager(const std::wstring& windowName, unsigned int width, unsigned int height);
	~WindowsManager() override;

	void Initialize() override;
	static std::optional<int> ProcessMessages() noexcept;
	void Shutdown() override;

	//~ Getters
	[[nodiscard]] HINSTANCE GetHandleInstance() const;
	[[nodiscard]] HWND GetHandleWindow() const;
	[[nodiscard]] std::wstring GetWindowName() const;
	[[nodiscard]] unsigned int GetWidth() const;
	[[nodiscard]] unsigned int GetHeight() const;

	void EnableFreelook();
	void DisableFreelook();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

private:
	HINSTANCE	 mhInstance;
	HWND		 mHandleWindow;
	std::wstring mWindowName;
	unsigned int mWidth;
	unsigned int mHeight;
};
