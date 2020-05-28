#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "enum.hpp"
#include <opencv2/core/mat.hpp>

namespace Display
{

cv::Mat getHistogramDisplay(const cv::Mat& histogram, const Histogram mode = Histogram::Curve);

}

#endif // DISPLAY_HPP
