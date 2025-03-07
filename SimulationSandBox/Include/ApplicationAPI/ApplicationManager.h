#pragma once
#include <string>

/*
 * Entry Point hai application ka
 * Yhi se sb simulation shuru hota hai
 */
class ApplicationManager
{
public:
	ApplicationManager(const std::wstring& AppName, int width, int height);
	virtual ~ApplicationManager() = default;

	int Update();
};

