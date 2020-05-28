#include "utils_impl.hpp"

namespace utils
{

void split(const cv::Mat& input, std::vector<cv::Mat>& splittedInput)
{
    switch(input.depth())
    {
    case CV_8U:
        split<uchar>(input, splittedInput);
        break;
    case CV_8S:
        split<char>(input, splittedInput);
        break;
    case CV_16U:
        split<ushort>(input, splittedInput);
        break;
    case CV_16S:
        split<short>(input, splittedInput);
        break;
    case CV_32S:
        split<int>(input, splittedInput);
        break;
    case CV_32F:
        split<float>(input, splittedInput);
        break;
    case CV_64F:
        split<double>(input, splittedInput);
        break;
    }
}

void merge(const std::vector<cv::Mat>& inputs, cv::Mat& mergedInputs)
{
    CV_Assert(inputs.size() > 0);
    switch(inputs[0].depth())
    {
    case CV_8U:
        merge<uchar>(inputs, mergedInputs);
        break;
    case CV_8S:
        merge<char>(inputs, mergedInputs);
        break;
    case CV_16U:
        merge<ushort>(inputs, mergedInputs);
        break;
    case CV_16S:
        merge<short>(inputs, mergedInputs);
        break;
    case CV_32S:
        merge<int>(inputs, mergedInputs);
        break;
    case CV_32F:
        merge<float>(inputs, mergedInputs);
        break;
    case CV_64F:
        merge<double>(inputs, mergedInputs);
        break;
    }
}

cv::Mat getMinMaxValue(const cv::Mat& input)
{
    switch(input.depth())
    {
    case CV_8U:
        return getMinMaxValue<uchar>(input);
    case CV_8S:
        return getMinMaxValue<char>(input);
    case CV_16U:
        return getMinMaxValue<ushort>(input);
    case CV_16S:
        return getMinMaxValue<short>(input);
    case CV_32S:
        return getMinMaxValue<int>(input);
    case CV_32F:
        return getMinMaxValue<float>(input);
    case CV_64F:
        return getMinMaxValue<double>(input);
    }
}

}
