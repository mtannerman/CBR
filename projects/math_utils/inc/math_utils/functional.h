#pragma once
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include "common/exceptions.h"

namespace cbr {
namespace f {
// TODO: second template: plus function with default std::plus
template <typename Coll>
typename Coll::value_type sum(const Coll& collection) {
    using T = typename Coll::value_type;

	T sum{};
    for (auto it = begin(collection); it != end(collection); ++it) {
        sum += *it;
    }

    return sum;
}

template <typename Coll, typename F>
decltype(auto) sum(const Coll& collection, F f) {
    using T = typename std::result_of<F(typename Coll::value_type)>::type;

	T sum{};
    for (auto it = begin(collection); it != end(collection); ++it) {
        sum += f(*it);
    }

    return sum;
}

template <typename It, typename F, typename P>
decltype(auto) map_if(const It& itBegin, const It& itEnd, const F& mapper,
                       const P& predicate) {
    using T = typename std::result_of<F(
        typename std::iterator_traits<It>::value_type)>::type;

    if (itBegin == itEnd) {
        return std::vector<T>();
    }

    std::vector<T> newColl;
    newColl.reserve(distance(itBegin, itEnd));

    for (auto it = itBegin; it != itEnd; ++it) {
        if (predicate(*it)) {
            newColl.emplace_back(mapper(*it));
        }
    }

    return newColl;
}

template <typename C, typename F, typename P>
decltype(auto) map_if(const C& collection, const F& mapper,
                       const P& predicate) {
    return map_if(begin(collection), end(collection), mapper, predicate);
}

template <typename C, typename Pred>
void filter(C& collection, const Pred& predicate) {
    const auto& size = collection.size();
    if (size == 0) {
        return;
    }

    for (auto it = begin(collection); it != end(collection);) {
        if (predicate(*it)) {
            ++it;
        } else {
            it = collection.erase(it);
        }
    }
}

template <typename It, typename F>
decltype(auto) map(const It& itBegin, const It& itEnd, const F& function) {
    using T = typename std::result_of<F(
        typename std::iterator_traits<It>::value_type)>::type;

    if (itBegin == itEnd) {
        return std::vector<T>();
    }

    std::vector<T> newColl;
    newColl.reserve(distance(itBegin, itEnd));

    for (auto it = itBegin; it != itEnd; ++it) {
        newColl.emplace_back(function(*it));
    }

    return newColl;
}

template <typename C, typename F>
decltype(auto) map(const C& collection, const F& function) {
    return map(begin(collection), end(collection), function);
}

template <typename C, typename F>
decltype(auto) average_difference(const C& collection, const F& difference) {
    using T = typename std::result_of<F(typename C::value_type,
                                        typename C::value_type)>::type;
    const auto& size = collection.size();
    if (size < 2) {
        return T(0);
    }

    T sum{};

    auto prev = begin(collection);
    for (auto it = next(begin(collection)); it != end(collection); ++it) {
        sum += difference(*prev, *it);
        prev = it;
    }

    return (sum / T(size - 1));
}

template <typename C, typename F>
decltype(auto) difference_sum(const C& collection, const F& difference) {
    using T = typename std::result_of<F(typename C::value_type,
                                        typename C::value_type)>::type;

    const auto& size = collection.size();

    if (size < 2) {
        return T{};
    }

    T sum(0);

    auto prev = begin(collection);
    auto endIt = end(collection);
    for (auto it = next(begin(collection)); it != endIt; ++it) {
        sum += difference(*prev, *it);
        prev = it;
    }

    return sum;
}

template <typename C, typename F>
decltype(auto) difference_vector(const C& collection, const F& difference) {
    using T = typename std::result_of<F(typename C::value_type,
                                        typename C::value_type)>::type;
    const auto& size = collection.size();
    if (size < 2) {
        return std::vector<T>();// fails if T is not default constructible
    }

    std::vector<T> differenceVector;
    differenceVector.reserve(size - 1);

    auto prev = begin(collection);
    for (auto it = next(begin(collection)); it != end(collection); ++it) {
        differenceVector.emplace_back(difference(*prev, *it));
        prev = it;
    }

    return differenceVector;
}

template <typename C1, typename C2, typename F>
decltype(auto) combine(const C1& collection1, const C2& collection2,
                        const F& f) {
    using T = typename std::result_of<F(typename C1::value_type,
                                        typename C2::value_type)>::type;

    const size_t size1 = collection1.size();
    const size_t size2 = collection2.size();


	THROW_IF(size1 != size2, SizeMismatch, "");

    std::vector<T> combined;
    combined.reserve(size1);

    auto it1 = begin(collection1);
    auto it2 = begin(collection2);
    auto end1 = end(collection1);
    while (it1 != end1) {
        combined.emplace_back(f(*it1, *it2));

        ++it1;
        ++it2;
    }

    return combined;
}

template<typename Collection>
std::string collection_to_string(
	const Collection& coll, 
	const std::string delim = ", ", 
	const std::string leftBoundary = "", 
	const std::string rightBoundary = "")
{
	if (size(coll) == 0) {
		return "";
	}

	std::stringstream ss;
	ss << leftBoundary;
	for (auto it = begin(coll); it != prev(end(coll)); ++it) {
		ss << *it << delim;
	}
	ss << *prev(end(coll)) << rightBoundary;

	return ss.str();
}

template<typename Collection, typename ToStringF>
std::string collection_to_string_f(
	const Collection& coll, 
	const ToStringF toString, 
	const std::string delim = ", ", 
	const std::string leftBoundary = "", 
	const std::string rightBoundary = "")
{
	if (size(coll) == 0) {
		return "";
	}

	std::stringstream ss;
	ss << leftBoundary;
	for (auto it = begin(coll); it != prev(end(coll)); ++it) {
		ss << toString(*it) << delim;
	}
	ss << toString(*prev(end(coll))) << rightBoundary;

	return ss.str();
}
template <typename Collection>
void FillCollection(Collection& coll, const typename Collection::value_type& value)
{
    for (auto& elem : coll) {
        elem = value;
    }
}

}   // namespace f
}   // namespace cbr