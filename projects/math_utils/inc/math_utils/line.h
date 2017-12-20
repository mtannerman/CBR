#pragma once
#include "opencv2/core.hpp"
#include "common/logging.h"

namespace cbr
{

template <typename T>
struct Line2d
{
    typedef cv::Point_<T> PointT;
    Line2d() = default;
    static Line2d FromTwoPointsOnLine(const PointT& p1, const PointT& p2)
    {
        Line2d ret;
        ret.P = p1;
        ret.v = p2 - p1;
        return ret;
    }

    static Line2d FromPointAndDirection(const PointT& P_, const PointT& v_)
    {
        Line2d ret;
        ret.P = P_;
        ret.v = v_;
        return ret;
    }

    static Line2d FromAngle(const T angle)
    {
        Line2d ret;
        ret.P = PointT();
        ret.v = PointT(std::cos(angle), std::sin(angle));
        return ret;
    }

    cv::Point2d At(const double t) const
    {
        return cv::Point2d(P) + t * cv::Point2d(v);
    }

    double ClosestTimeArg(const PointT& otherPoint) const
    {
        const double numerator = (otherPoint - P).ddot(v);
        const double denomRoot = cv::norm(v);

        return numerator / (denomRoot * denomRoot);
    }

    PointT ClosestPoint(const PointT& otherPoint) const
    {
        const double tMin = ClosestTimeArg(otherPoint);
        return P + PointT(cv::Point2d(v) * tMin);
    }

    double DistanceFromPoint(const PointT& otherPoint) const
    {
        const double tMin = ClosestTimeArg(otherPoint);
        return cv::norm(cv::Point2d(otherPoint) - cv::Point2d(P) - cv::Point2d(v) * tMin);
    }

    cv::Point2d Intersection(const Line2d& other) const
    {
        const auto det = v.x * other.v.y - v.y * other.v.x;
        // LOG("--=-==-=-=-=-=-");
        // LOG(DESC(det));
        if (std::abs(det) < 1e-7) return cv::Point2d(1e50, 1e50);

        const auto timeArg = (other.v.y * (other.P.x - P.x) - other.v.x * (other.P.y - P.y)) / det;
        // LOG(DESC(timeArg));
        return At(timeArg);
    }

    PointT P;
    PointT v;

};

}