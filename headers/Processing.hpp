#ifndef PROCESSING_HPP
#define PROCESSING_HPP

#include "Processing_impl.hpp"

namespace Processing
{

/**
 * @brief Computes the histogram of an image
 * @param input : the input image
 * @param mode : the histogram to compute (normal, row, column, cumulative)
 * @param rowColIdx : row or column index to compute in case of row or column mode
 * @return the computed histogram
 */
cv::Mat computeImageHistogram(const cv::Mat& input, const Histogram mode, const int rowColIdx = -1);

/**
 * @brief Color conversion of an image
 * @param input : the input image
 * @param output : the output color converted image
 * @param mode : the conversion mode of colors
 */
void convertImageColor(const cv::Mat& input, cv::Mat& output, const Conversion mode);

};

#endif // PROCESSING_HPP
