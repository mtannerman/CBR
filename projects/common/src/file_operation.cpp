#include "common/file_operation.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <cstdlib>
#include "common/logging.h"
#include "common/exceptions.h"
#include <algorithm>
#include <iostream>

// could use _MSC_VER for this
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define getcwd _getcwd // MSFT "deprecation" warning
#else
#include <unistd.h>
#endif

namespace cbr {

bool IsDirectoryOrFileExist(const std::string& path)
{
    struct stat info;
    return stat(path.c_str(), &info) == 0;
}

bool IsDirectory(const std::string& path) 
{
    if (IsDirectoryOrFileExist(path)) {
        struct stat info;
        stat(path.c_str(), &info);
		auto ret = int(info.st_mode) & S_IFDIR;
        return ret != 0;
    }

    return false;
}

bool CbrCreateDirectory(const std::string& path) 
{
#if defined(__GNUG__)
	return system(STR("mkdir -p " << path).c_str()) == 0;
#endif
#if defined(_WIN32)
	if (!IsDirectoryOrFileExist(path)) {
		std::string pathWithBackSlashes = path;
		std::replace(pathWithBackSlashes.begin(), pathWithBackSlashes.end(), '/', '\\');
		const auto command = STR("md " << pathWithBackSlashes);
		std::cout << command << std::endl;
		system(command.c_str());
	}
	/*const auto ret = CreateDirectoryA(path.c_str(), NULL);
	std::cout << ret << std::endl;
	if (ret == EEXIST) {
		std::cout << "Directory was not created because " << path << " is the name of an existing file, directory, or device." << std::endl;
	}
	else if (ret == ENOENT) {
		std::cout << "Path was not found." << std::endl;
	}
	else if (ret == EPERM) {
		std::cout << "Permission problems during directory creation." << std::endl;
	}*/
	return IsDirectoryOrFileExist(path);
#endif
	THROW(UnImplementedFeature, "");
	return false;
}

std::string GetWorkingDirectory()
{
    char buffer[1024];
    ASSERT(getcwd(buffer, sizeof(buffer)) != NULL, "");
    std::string buildDirectory(buffer);
#if defined(_WIN32)
	std::replace(buildDirectory.begin(), buildDirectory.end(), '\\', '/');
#endif
	const auto buildDirectoryPosition = buildDirectory.find("/build");
    const auto withoutBuildAtTheEnd = buildDirectory.substr(0, buildDirectoryPosition);
	std::cout << "need to test this with gcc: " << DESC(withoutBuildAtTheEnd) << std::endl;
    return withoutBuildAtTheEnd;
}

std::string GetProcessDirectory()
{
    return GetWorkingDirectory() + "/process";
}

std::string GetInputDirectory()
{
    return GetWorkingDirectory() + "/input";
}

}