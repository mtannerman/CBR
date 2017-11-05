#pragma once

#include <string>

namespace cbr {

bool IsDirectoryOrFileExist(const std::string& path);
bool IsDirectory(const std::string& path);
bool CreateDirectory(const std::string& path);

}