#include "Display_impl.hpp"
#include "utils_impl.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
namespace Display
{
namespace impl
{
namespace
{
const cv::Scalar_<uchar> k_oneChannelHistogramColors(0); // Black
const std::array<cv::Scalar_<uchar>, 4> k_multiChannelHistogramColors = { cv::Scalar_<uchar>(255, 0, 0), // Blue
                                                                          cv::Scalar_<uchar>(0, 255, 0), // Green
                                                                          cv::Scalar_<uchar>(0, 0, 255), // Red
                                                                          cv::Scalar_<uchar>::all(100)
                                                                        }; // Grey
}

cv::Mat getHistogramDisplay(const cv::Mat& histogram, const bool curve)
{
    // Get the max value of the histogram to normalize the display
    const int max = utils::getMinMaxValue(histogram).at<int>(1, 0);

    cv::Mat display(1024, 1024, CV_8UC3, cv::Scalar::all(255));

    std::vector<cv::Mat> histograms;
    utils::split(histogram, histograms);

    for (unsigned int i = 0; i < histograms.size(); i++)
    {
        const int* histogramPtr = histograms[i].ptr<int>(0);
        const cv::Mat minMax = utils::getMinMaxValue(histograms[i]);

        if (curve)
        {
            for (int bin = 0; bin < 255; bin++)
                cv::line(display, cv::Point2i(bin * 4, display.rows - static_cast<int>(histogramPtr[bin] * (display.rows * 0.95) / max)),
                         cv::Point2i((bin + 1) * 4, display.rows - static_cast<int>(histogramPtr[bin + 1] * (display.rows * 0.95) / max)),
                         (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, 8, 0);
        }
        else // Bar histogram
        {
            for (int bin = 0; bin < 256; bin++)
                cv::line(display, cv::Point2i(bin * 4 + static_cast<int>(i), display.rows),
                         cv::Point2i(bin * 4 + static_cast<int>(i), display.rows - static_cast<int>(histogramPtr[bin] * (display.rows * 0.95) / max)),
                         (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, 8, 0);
        }
    }

    // Display the max values
    for (unsigned int i = 0; i < histograms.size(); i++)
    {
        const cv::Mat minMax = utils::getMinMaxValue(histograms[i]);

        if (curve)
        {
            const cv::Point2i maxValuePos = cv::Point2i(minMax.at<int>(1, 1) * 4, display.rows - static_cast<int>(minMax.at<int>(1, 0) * (display.rows * 0.95) / max));
            cv::circle(display, maxValuePos, 3, (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, cv::LINE_AA);
            cv::putText(display, "(" + std::to_string(minMax.at<int>(1, 1)) + ", " + std::to_string(minMax.at<int>(1, 0)) + ")", maxValuePos + cv::Point2i(5, 0),
                        cv::FONT_HERSHEY_DUPLEX, 0.5, (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, cv::LINE_AA);
        }
        else
        {
            const cv::Point2i maxValuePos = cv::Point2i(minMax.at<int>(1, 1) * 4 + static_cast<int>(i), display.rows - static_cast<int>(minMax.at<int>(1, 0) * (display.rows * 0.95) / max));
            cv::circle(display, maxValuePos, 3, (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, cv::LINE_AA);
            cv::putText(display, "(" + std::to_string(minMax.at<int>(1, 1)) + ", " + std::to_string(minMax.at<int>(1, 0)) + ")", maxValuePos + cv::Point2i(5, 0),
                        cv::FONT_HERSHEY_DUPLEX, 0.5, (histogram.channels() > 1) ? k_multiChannelHistogramColors[i] : k_oneChannelHistogramColors, 1, cv::LINE_AA);
        }
    }

    return display;
}

}
}
