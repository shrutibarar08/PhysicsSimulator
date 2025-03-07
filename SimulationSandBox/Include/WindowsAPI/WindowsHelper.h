#pragma once
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <filesystem>

inline std::string NormalizePath(const std::string& path)
{
    std::string result = path;
    std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}

inline std::string OpenFileDialog()
{
    // Store the current working directory
    char currentDir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDir);
    std::filesystem::path basePath = currentDir; // Store base path as filesystem::path

    char filename[MAX_PATH] = { 0 };
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = GetActiveWindow();  // Avoid NULL
    ofn.lpstrFilter = "All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    std::string result;
    if (GetOpenFileNameA(&ofn))
    {
        std::cout << "Dialog returned: " << filename << "\n";
        std::filesystem::path fullPath = filename;

        // Convert to relative path if possible
        std::error_code ec;
        std::filesystem::path relativePath = std::filesystem::relative(fullPath, basePath, ec);

        if (!ec) // No error -> use relative path
            result = relativePath.string();
        else     // Fallback to full path if relative conversion fails
            result = fullPath.string();

        // Restore the working directory
        SetCurrentDirectoryA(currentDir);
    }
    else
    {
        std::cout << "Dialog canceled or failed.\n";
    }

    // Restore working directory even if dialog is canceled
    SetCurrentDirectoryA(currentDir);

    return NormalizePath(result);
}
