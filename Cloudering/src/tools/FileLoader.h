#pragma once
#include <json/json.h>


class FileLoader {
public:
	static bool LoadJSON(Json::Value &root, std::string filePath);
};