#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>
namespace Utils {
#ifdef RUNNING_WINDOWS
#include <windows.h>
	inline std::string GetWorkingDirectory() {
		HMODULE hModule = GetModuleHandle(nullptr);
		if (hModule) {
			char path[256];
			GetCurrentDirectory(256, path);
			return std::string(path) + "\\"; // new
		}
		return "";
	}
#elif defined RUNNING_LINUX
#include <unistd.h>
	inline std::string GetWorkingDirectory() {
		char cwd[1024];
		if (getcwd(cwd, sizeof(cwd)) != nullptr) {
			return std::string(cwd) + std::string("/");
		}
		return "";
	}
#endif
}