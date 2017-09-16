#pragma once

#include "common/logging.h"

#define ASSERT(expression, message) if (!(expression)) { ::cbr::throw_exception(STR(message), __FUNCTION__, __FILE__, __LINE__); }

namespace cbr
{
    void throw_exception(const std::string& message,
        const std::string& function, 
        const std::string& file, 
        const int line);
}