#pragma once

#include <memory>
#include <experimental/propagate_const>

namespace cbr
{
    template <typename T>
    using impl_ptr = std::experimental::propagate_const<std::unique_ptr<T>>;
}