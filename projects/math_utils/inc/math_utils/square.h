#pragma once

#include <array>
#include <vector>
#include "opencv2/core.hpp"
#include <cstddef>
#include "math_utils/point.h"

namespace cbr
{

struct Square
{
    Square() = default;

    void Recenter();

    bool Contains(const Point& p) const;
    double Distance(const Point& p) const;

    Point& operator[](const size_t idx); 
    const Point& operator[](const size_t idx) const;
    double Circumference() const;
    std::array<Point, 4> corners;
    Point middle;
};

}