#include "common/archive_directory.h"

namespace cbr
{

std::string ArchiveDirectory::Get(const std::string innerDirectoryName) const
{
    if (innerDirectoryName == "") {
        return STR(GetWorkingDirectory() << "/process/" << mDateTimeStr);
    }
    const auto fullDirName = STR(GetWorkingDirectory() <<  "/process/" << mDateTimeStr << "/" << innerDirectoryName);
    THROW_IF(!CbrCreateDirectory(fullDirName), FileOperationFailure, STR("creating directory failed:" << fullDirName));
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