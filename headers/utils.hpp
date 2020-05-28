#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/core/mat.hpp>

namespace utils
{

/**
 * @brief Extract a single row from a matrix
 * @param input : the input matrix
 * @param rowIndex : the index of the row to extract
 * @return an input.cols*1 matrix
 */
cv::Mat extractRow(const cv::Mat& input, const int rowIndex);

/**
 * @brief Splits an N channels matrix into a vector of N matrixes
 * @param input : the input to split
 * @param splittedInput : the vector containing the splitted channels
 */
template<class T>
void split(const cv::Mat& input, std::vector<cv::Mat>& splittedInput);
void split(const cv::Mat& input, std::vector<cv::Mat>& splittedInput);

/**
 * @brief Merges a vector of N matrixes into an N channel matrix
 * @param inputChannels : the vector of channels to merge
 * @param merged : the matrix containing the merged N channel
 */
template<class T>
void merge(const std::vector<cv::Mat>& inputs, cv::Mat& mergedInputs);
void merge(const std::vector<cv::Mat>& inputs, cv::Mat& mergedInputs);

/**
 * @brief Get the min and max value and location of a matrix
 * @param input : the input matrix
 * @return a 4x2 matrix :
 *
 *             value     row     col    depth
 *      min [ (0, 0)   (0, 1)  (0, 2)  (0, 3) ]
 *      max [ (1, 0)   (1, 1)  (1, 2)  (1, 3) ]
 */
template<class T>
cv::Mat getMinMaxValue(const cv::Mat& input);
cv::Mat getMinMaxValue(const cv::Mat& input);

};

#include "../src/utils.tpp"

#endif // UTILS_HPP
