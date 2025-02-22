#include "FileLoader.h"
#include <fstream>

bool FileLoader::LoadJSON(Json::Value &root, std::string filePath) {
    std::ifstream file(filePath);
    if (!file) return false;

    file >> root;
    file.close();

    return true;
}