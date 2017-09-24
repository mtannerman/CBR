#pragma once
#include "opencv2/core.hpp"

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

    PointT P;
    PointT v;

};

}