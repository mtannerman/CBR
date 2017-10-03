#pragma once

#include <vector>
#include <cstddef>

namespace cbr
{

template <typename T>
class CyclicContainer
{
public:
    template <typename Collection>
    static CyclicContainer Construct(const Collection& collection)
    {
        CyclicContainer ret;
        for (const auto& elem : collection) {
            ret.elements.push_back(elem);
        }
        return ret;
    }

    const T& Get() const 
    {
        const auto idxCpy = idx;
        idx += 1;
        if (idx == elements.size())  {
            idx = 0;
        }

        return elements[idxCpy];
    }

private:
    CyclicContainer() = default;
    mutable size_t idx = 0;
    std::vector<T> elements;
};

}   // namespace cbr