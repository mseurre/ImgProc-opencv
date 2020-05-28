#include "Display.hpp"
#include "Display_impl.hpp"

namespace Display
{

cv::Mat getHistogramDisplay(const cv::Mat& histogram, const Histogram mode)
{
    switch (mode)
    {
    case Histogram::Curve:
        return impl::getHistogramDisplay(histogram, true);
    case Histogram::Bar:
        return impl::getHistogramDisplay(histogram, false);
    }
}

}
