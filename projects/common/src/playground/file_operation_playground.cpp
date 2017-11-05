#include "common/playground/file_operation_playground.h"
#include <cstdlib>
#include <string>
#include "common/logging.h"
#include "sys/types.h"
#include "sys/stat.h"


namespace cbr {
namespace playground {
        
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
    return system(STR("mkdir -p " << path).c_str()) == 0;
}

void StartFileOperationPlayground()
{
    #if defined(__GNUG__)
        // LOG(DESC(IsDirectoryOrFileExist("../process/template")));
        // LOG(DESC(IsDirectoryOrFileExist("../process/template1")));
        // LOG(DESC(IsDirectoryOrFileExist("../process/template2")));
        // LOG(DESC(IsDirectoryOrFileExist("../process/template/f.txt")));
        // LOG(DESC(IsDirectoryOrFileExist("../process/template/g.txt")));
        // LOG(DESC(IsDirectoryOrFileExist("../process/template2/f.txt")));
        // LOG("*****");
        // LOG(DESC(IsDirectory("../process/template")));
        // LOG(DESC(IsDirectory("../process/template1")));
        // LOG(DESC(IsDirectory("../process/template2")));
        // LOG(DESC(IsDirectory("../process/template/f.txt")));
        // LOG(DESC(IsDirectory("../process/template/g.txt")));
        // LOG(DESC(IsDirectory("../process/template2/f.txt")));

        // LOG("gnug");

        // LOG("creating directory");
        // const int dirErr = system("mkdir -p ../process/template");
        // LOG(DESC(dirErr));

        // LOG(DESC(CreateDirectory("../process/template2")));
        // LOG(DESC(CreateDirectory("../process/template3")));
        // LOG(DESC(CreateDirectory("../process/template2/template2p1")));
        LOG(DESC(__FUNCTION__));
        LOG(DESC(__PRETTY_FUNCTION__));
        auto l = [](){ LOG("in l()"); };
        l();

        // LOG(DESC());
    #endif


    
}
    
}
}