#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <map>
#include <Windows.h>
#include <string>
#include <ostream>
#include <fstream>
#include <filesystem>

class Utils {
public:
	//
	inline static std::string getRoamingStatePath() {
		static const std::string path = (getenv("AppData") + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\"));
		return path;
	}

	inline static std::string getClientPath() {
		static const std::string path = getRoamingStatePath() + "Alas\\";
		return path;
	}

	inline static bool doesClientPathExist(const std::string& path) {
		return std::filesystem::exists(path);
	}

	inline static bool createPath(const std::string& path) {
		return std::filesystem::create_directory(path);
	}

	inline static void deletePath(const std::string& path) {
		std::filesystem::remove(path);
	}
};