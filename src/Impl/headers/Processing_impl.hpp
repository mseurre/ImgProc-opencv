#ifndef PROCESSING_IMPL_HPP
#define PROCESSING_IMPL_HPP

#include "enum_impl.hpp"

#include <opencv2/core/mat.hpp>

namespace Processing
{
namespace impl
{

/** @brief Conversion of a single color between color spaces
 * @param inputColor : the input CV_8U image (channels must be 1, 3 or 4)
 * @param from : enum value for the input colorspace
 * @param to : enum value for the output colorspace
 * @return the converted color as a scalar
 */
cv::Scalar_<uchar> convertColor(const cv::Scalar_<uchar>& inputColor, const ColorSpace from, const ColorSpace to);

/** @brief Conversion between color spaces
 * @param input : the input CV_8U image (channels must be 1, 3 or 4)
 * @param output : the output CV_8U image (nb of channels depends on the conversion)
 * @param from : enum value for the input colorspace
 * @param to : enum value for the output colorspace
 */
void convertImageColor(const cv::Mat& input, cv::Mat& output, const ColorSpace from, const ColorSpace to);

/** @brief Compute the histogram of an image
 * @param input : the input CV_8U image
 * @return a 1*256*n bins histogram of the input (n = input.channels())
 */
cv::Mat computeImageHistogram(const cv::Mat& input);

/**
 * @brief Compute the histogram of a single row of an image
 * @param input : the input CV_8U image
 * @param rowIndex : the index of the row
 * @return a 1*256*n bins histogram of the input (n = input.channels())
 */
cv::Mat computeImageRowHistogram(const cv::Mat& input, const int rowIndex);

/**
 * @brief Compute the histogram of a single column of an image
 * @param input : the input CV_8U image
 * @param columnIndex : the index of the column
 * @return a 1*256*n bins histogram of the input (n = input.channels())
 */
cv::Mat computeImageColumnHistogram(const cv::Mat& input, const int columnIndex);

/** @brief Compute the cumulative histogram of an image
 * @param input : the input CV_8U image
 * @return a 1*256*n bins cumulative histogram of the input (n = input.channels())
 */
cv::Mat computeImageCumulativeHistogram(const cv::Mat& input);

}
}

#endif // PROCESSING_IMPL_HPP
