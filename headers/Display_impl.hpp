#ifndef DISPLAY_IMPL_HPP
#define DISPLAY_IMPL_HPP

#include <opencv2/core/mat.hpp>

namespace Display
{
namespace impl
{

cv::Mat getHistogramDisplay(const cv::Mat& histogram, const bool curve);

}
}

#endif // DISPLAY_HPP
