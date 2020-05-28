#include "Processing.hpp"

namespace Processing
{

cv::Mat computeImageHistogram(const cv::Mat& input, const Histogram mode, const int rowColIdx)
{
    switch (mode)
    {
        case Histogram::Normal:
            return impl::computeImageHistogram(input);

        case Histogram::Row:
            return impl::computeImageRowHistogram(input, rowColIdx);

        case Histogram::Column:
            return impl::computeImageColumnHistogram(input, rowColIdx);

        case Histogram::Cumulative:
            return impl::computeImageCumulativeHistogram(input);
    }
}

void convertImageColor(const cv::Mat& input, cv::Mat& output, const Conversion mode)
{
    switch (mode)
    {
        case Conversion::BGR2Grey_Average:
            impl::convertImageColor(input, output, impl::ColorSpace::BGR, impl::ColorSpace::Grey_Average);
            break;

        case Conversion::BGR2Grey_Lightness:
            impl::convertImageColor(input, output, impl::ColorSpace::BGR, impl::ColorSpace::Grey_Lightness);
            break;

        case Conversion::BGR2Grey_Luminosity:
            impl::convertImageColor(input, output, impl::ColorSpace::BGR, impl::ColorSpace::Grey_Luminosity);
            break;

        case Conversion::Grey2BGR:
            impl::convertImageColor(input, output, impl::ColorSpace::Grey_Average, impl::ColorSpace::BGR);
            break;
    }
}

}
