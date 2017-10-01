#pragma once

#include <memory>

namespace cbr
{
    template <typename T>
    using impl_ptr = std::unique_ptr<T>;
}