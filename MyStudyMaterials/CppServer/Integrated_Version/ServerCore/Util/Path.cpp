#include "Path.h"

#include <filesystem>
#include <stdexcept>


#ifdef _WIN32
#include <windows.h> // GetModuleFileNameA
#else
#include <unistd.h> // readlink
#endif

namespace ServerCore
{
std::string Path::GetExecutablePath()
{
	char buffer[1024];
	size_t size = sizeof(buffer);
#ifdef _WIN32
	if (GetModuleFileNameA(NULL, buffer, size) == 0)
	{
		throw std::runtime_error("Unable to get executable path");
	}
#else
	ssize_t count = readlink("/proc/self/exe", buffer, size);
	if (count == -1) {
		throw std::runtime_error("Unable to get executable path");
	}
	buffer[count] = '\0';
#endif
	return std::string(buffer);
}

std::string Path::GetRelativeFilePath(const std::string& _filePath)
{
	std::filesystem::path exePath = GetExecutablePath();
	std::filesystem::path exeDir = exePath.parent_path();
	
	std::filesystem::path relPath(_filePath);
	for (auto& part : relPath)
	{
		if (part == "..")
			exeDir = exeDir.parent_path();
		else
			exeDir /= part;

	}

	return exeDir.lexically_normal().string();
}
}
