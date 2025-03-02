#include "FileLoader.h"
#include <fstream>
#include <windows.h>
#include <commdlg.h>
#include <stdexcept>
#include <iostream>
#include <string>

bool FileLoader::LoadJSON(Json::Value &root, std::string filePath) {
    std::ifstream file(filePath);
    if (!file) return false;

    file >> root;
    file.close();

    return true;
}


std::string FileLoader::OpenFileDialog(std::string preferedType)
{
    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH] = { 0 };

    std::vector<wchar_t> filter;

    auto AppendFilter = [&filter](const std::wstring& name, const std::wstring& ext) {
        filter.insert(filter.end(), name.begin(), name.end());
        filter.push_back(L'\0'); // Null-terminate name
        filter.insert(filter.end(), ext.begin(), ext.end());
        filter.push_back(L'\0'); // Null-terminate extension
        };

    AppendFilter(L"Expected File Ext (*." + StringToWString(preferedType) + L")", L"*." + StringToWString(preferedType));
    AppendFilter(L"Basic Pipeline (*.vert;*.frag)", L"*.vert;*.frag");
    AppendFilter(L"Tessellation Shaders (*.tesc;*.tese)", L"*.tesc;*.tese");
    AppendFilter(L"Geometry Shader (*.geom)", L"*.geom");
    AppendFilter(L"All Shader Ext (*.vert;*.tesc;*.tese;*.geom;*.frag)", L"*.vert;*.tesc;*.tese;*.geom;*.frag");
    AppendFilter(L"All Files", L"*.*");

    filter.push_back(L'\0');

    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter.data();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Select a Shader File";

    if (GetOpenFileName(&ofn) == TRUE)
    {
        return WStringToString(szFile);
    }
    return "";
}

std::string FileLoader::WStringToString(const std::wstring& wstr)
{
    if (wstr.empty()) return "";

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], sizeNeeded, NULL, NULL);

    return str;
}

std::wstring FileLoader::StringToWString(const std::string& str) 
{
    if (str.empty()) return L"";

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed-1, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

    return wstr;
}