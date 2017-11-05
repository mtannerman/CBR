#include "common/file_operation.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <cstdlib>
#include "common/logging.h"

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
        return info.st_mode & S_IFDIR;
    }

    return false;
}

bool CreateDirectory(const std::string& path) 
{
    #if defined(__GNUG__)
        return system(STR("mkdir -p " << path).c_str()) == 0;
    #endif

    return false;
}    

}