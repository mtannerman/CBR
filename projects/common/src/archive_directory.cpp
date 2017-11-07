#include "common/archive_directory.h"

namespace cbr
{

std::string ArchiveDirectory::Get(const std::string innerDirectoryName) const
{
    if (innerDirectoryName == "") {
        return STR("../process/" << mDateTimeStr);
    }
    const auto fullDirName = STR("../process/" << mDateTimeStr << "/" << innerDirectoryName);
    ASSERT(CreateDirectory(fullDirName), STR("creating directory failed:" << fullDirName));
    return fullDirName;
}

ArchiveDirectory& ArchiveDirectory::GetInstance()
{
    static ArchiveDirectory instance;
    return instance;
}

ArchiveDirectory::ArchiveDirectory()
{
    mDateTimeStr = GetDateTimeStr();
}

}