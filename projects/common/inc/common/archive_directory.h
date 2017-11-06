#pragma once

#include "common/time.h"
#include <string>
#include "common/logging.h"
#include "common/file_operation.h"
#include "common/exceptions.h"

namespace cbr
{

class ArchiveDirectory
{
public:
    std::string Get(const std::string innerDirectoryName = std::string("")) const;
    static ArchiveDirectory& GetInstance();
private:
    ArchiveDirectory();
    std::string mDateTimeStr;
};

}