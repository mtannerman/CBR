#pragma once

#include <string>



namespace cbr {

bool IsDirectoryOrFileExist(const std::string& path);
bool IsDirectory(const std::string& path);
bool CbrCreateDirectory(const std::string& path);
std::string GetWorkingDirectory();
std::string GetProcessDirectory();
std::string GetInputDirectory();

}