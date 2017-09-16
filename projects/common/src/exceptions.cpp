#include "common/exceptions.h"

namespace cbr
{
void throw_exception(const std::string &message,
                     const std::string &function,
                     const std::string &file,
                     const int line)
{
        const std::string fullMessage = STR("ASSERTION" << "\n" <<
            '\t' << "File: " << file << "\n" <<
            '\t' << "Line: " << line << "\n" <<
            '\t' << "Function: " << function << "\n" <<
            '\t' << "Message: " << message;

        throw std::runtime_error(fullMessage);
}
}