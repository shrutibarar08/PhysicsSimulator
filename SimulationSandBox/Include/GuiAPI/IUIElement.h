#pragma once

#include <string>
#include <functional>
#include <memory>

#include "imgui/imgui.h"

class IUIElement
{
public:
	virtual ~IUIElement() = default;
	virtual void Render() = 0;
};


class MenuItem: public IUIElement
{
public:

	MenuItem(const std::string& label, std::function<void()> callback)
		: mLabel(label), mCallback(callback)
	{}

	void Render() override
	{
		if (ImGui::MenuItem(mLabel.c_str()))
		{
			if (mCallback) mCallback();
		}
	}

private:
	std::string mLabel;
	std::function<void()> mCallback;
};

class Menu: public IUIElement
{
public:

	Menu(const std::string& text) : mLabel(text)
	{}

	void AddItem(std::unique_ptr<IUIElement> item)
	{
		mChildren.emplace_back(std::move(item));
	}

	void Render() override
	{
		if (ImGui::BeginMenu(mLabel.c_str()))
		{
			for (auto& child : mChildren)
			{
				child->Render();  // Simulation menu items or submenus
			}
			ImGui::EndMenu();
		}
	}

private:
	std::string mLabel;
	std::vector<std::unique_ptr<IUIElement>> mChildren;

};

class MainMenuBar : public IUIElement
{
public:

	void AddMenu(std::unique_ptr<IUIElement> menu)
	{
		mMenuOption.emplace_back(std::move(menu));
	}

	void Render() override
	{
		if (ImGui::BeginMainMenuBar())
		{
			for (auto& menu : mMenuOption)
			{
				menu->Render();
			}
			ImGui::EndMainMenuBar();
		}
	}

private:
	std::vector<std::unique_ptr<IUIElement>> mMenuOption;
};

