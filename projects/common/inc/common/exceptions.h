#pragma once

#include "common/logging.h"

#define THROW(message) ::cbr::throw_exception(STR(message), __FUNCTION__, __FILE__, __LINE__);
#define ASSERT(expression, message) if (!(expression)) { THROW(message); }
#define THROW_IF(expression, message) if ((expression)) { THROW(message); }

namespace cbr
{
    void throw_exception(const std::string& message,
        const std::string& function, 
        const std::string& file, 
        const int line);
}