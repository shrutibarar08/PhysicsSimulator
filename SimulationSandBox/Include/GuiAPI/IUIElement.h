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

class SubOptionElement
{
public:
	static void DrawSubOption(const std::string& optionName, const std::function<void()>& fn)
	{
		// Custom header colors
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.75f, 0.0f, 1.0f)); // Orange text

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));

		if (ImGui::CollapsingHeader(optionName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			// Inner content styling
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2f, 0.2f, 0.2f, 0.6f));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);

			ImGui::BeginChild(("##" + optionName).c_str(), ImVec2(0, 0), true, ImGuiWindowFlags_None);

			fn(); // Call the passed function

			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
		}
	}
};
