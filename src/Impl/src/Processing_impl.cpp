#include "Processing_impl.hpp"
#include "utils_impl.hpp"

namespace Processing
{
namespace impl
{

cv::Mat computeImageHistogram(const cv::Mat& input)
{
    CV_Assert(input.depth() == CV_8U);
    CV_Assert(input.channels() != 2);

    if (input.channels() > 1)
    {
        // In case of a multi-channel image, process each histogram separately and merge the results
        std::vector<cv::Mat> splittedChannels, histograms;
        utils::split(input, splittedChannels);

        for (const cv::Mat& channel : splittedChannels)
            histograms.push_back(computeImageHistogram(channel));

        cv::Mat mergedHistogram;
        utils::merge(histograms, mergedHistogram);
        return mergedHistogram;
    }

    // The histogram will contain 256 bins, one for each value between 0 - 255
    cv::Mat histogram = cv::Mat::zeros(1, 256, CV_32SC1);
    int* histogramPtr = histogram.ptr<int>(0);

    // For each value of the input, add 1 to the corresponding bin
    for (unsigned int i = 0; i < input.total(); i++)
        histogramPtr[input.data[i]]++;

    return histogram;
}

cv::Mat computeImageRowHistogram(const cv::Mat& input, const int rowIndex)
{
    CV_Assert(rowIndex >= 0 && rowIndex < input.rows);
    return computeImageHistogram(input.row(rowIndex));
}

cv::Mat computeImageColumnHistogram(const cv::Mat& input, const int columnIndex)
{
    return computeImageRowHistogram(input.t(), columnIndex);
}

cv::Mat computeImageCumulativeHistogram(const cv::Mat& input)
{
    if (input.channels() > 1)
    {
        // In case of a multi-channel image, process each histogram separately and merge the results
        std::vector<cv::Mat> splittedChannels, histograms;
        utils::split(input, splittedChannels);

        for (const cv::Mat& channel : splittedChannels)
            histograms.push_back(computeImageCumulativeHistogram(channel));

        cv::Mat mergedHistogram;
        utils::merge(histograms, mergedHistogram);
        return mergedHistogram;
    }

    cv::Mat histo = computeImageHistogram(input);
    int* histoPtr = histo.ptr<int>(0);

    // For each bin, add the previous bin
    for (int i = 1; i < 256; i++)
        histoPtr[i] += histoPtr[i - 1];

    return histo;
}

cv::Scalar_<uchar> convertColor(const cv::Scalar_<uchar>& inputColor, const ColorSpace from, const ColorSpace to)
{
    switch (from)
    {
        case ColorSpace::BGR:
        {
            switch (to)
            {
                case ColorSpace::Grey_Average:
                    return cv::Scalar_<uchar>::all(static_cast<uchar>(0.33333333 * (inputColor[0] + inputColor[1] + inputColor[2])));

                case ColorSpace::Grey_Lightness:
                    return cv::Scalar_<uchar>::all(static_cast<uchar>((0.5 * (std::max(std::max(inputColor[0], inputColor[1]), inputColor[2]) +
                                                   std::min(std::min(inputColor[0], inputColor[1]), inputColor[2])))));

                case ColorSpace::Grey_Luminosity:
                    return cv::Scalar_<uchar>::all(static_cast<uchar>((0.114 * inputColor[0]) + (0.587 * inputColor[1]) + (0.299 * inputColor[2])));

                default:
                    break;
            }
        }

        case ColorSpace::Grey_Average:
        case ColorSpace::Grey_Lightness:
        case ColorSpace::Grey_Luminosity:
        {
            switch (to)
            {
                case ColorSpace::BGR:
                    return cv::Scalar_<uchar>::all(inputColor[0]);

                default:
                    break;
            }
        }
    }
}

void convertImageColor(const cv::Mat& input, cv::Mat& output, const ColorSpace from, const ColorSpace to)
{
    CV_Assert(input.depth() == CV_8U);
    CV_Assert(input.channels() != 2);

    switch (from)
    {
        case ColorSpace::BGR:
        {
            CV_Assert(input.type() == CV_8UC3);
            break;
        }

        case ColorSpace::Grey_Average:
        case ColorSpace::Grey_Lightness:
        case ColorSpace::Grey_Luminosity:
        {
            CV_Assert(input.type() == CV_8UC1);
            break;
        }
    }

    const cv::Mat inputClone = input.clone(); // Save input in case output = input

    switch (to)
    {
        case ColorSpace::Grey_Average:
        case ColorSpace::Grey_Lightness:
        case ColorSpace::Grey_Luminosity:
        {
            output.create(inputClone.size(), CV_8UC1);
            break;
        }

        case ColorSpace::BGR:
            output.create(inputClone.size(), CV_8UC3);
            break;
    }

    const uchar inputChannels = static_cast<uchar>(inputClone.channels());
    const uchar outputChannels = static_cast<uchar>(output.channels());
    cv::Scalar_<uchar> inputColor, outputColor;

    for (unsigned int i = 0; i < inputClone.total(); i++)
    {
        for (uchar c = 0; c < inputChannels; c++)
            inputColor[c] = inputClone.data[i * inputChannels + c];

        outputColor = convertColor(inputColor, from, to);

        for (uchar c = 0; c < outputChannels; c++)
            output.data[i * outputChannels + c] = outputColor[c];
    }
}

}
}
