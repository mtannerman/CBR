#pragma once
#include <type_traits>
#include <functional>

namespace cbr
{
    // TODO: second template: plus function with default std::plus
    template<typename Coll>
    typename Coll::value_type fsum(const Coll& collection)
    {
        using T = typename Coll::value_type;

        T sum(0);
        for (auto it = begin(collection); it != end(collection); ++it) {
            sum += *it;
        }

        return sum;
    }

    template<typename Coll, typename F>
    decltype(auto) fsum(const Coll& collection, F f)
    {
        using T = typename std::result_of<F(typename Coll::value_type)>::type;

        T sum(0);
        for (auto it = begin(collection); it != end(collection); ++it) {
            sum += f(*it);
        }

        return sum;
    }
}