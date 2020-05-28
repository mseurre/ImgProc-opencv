#include "Processing.hpp"
#include "Display.hpp"

#include <opencv2/highgui.hpp>

int main(int, char** argv)
{
    const cv::Mat input = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    if (input.empty())
        return -1;

    const cv::Mat inputHistogram = Processing::computeImageHistogram(input, Processing::Histogram::Normal);
    const cv::Mat histogramDisplay = Display::getHistogramDisplay(inputHistogram, Display::Histogram::Curve);

    cv::imshow("Input", input);
    cv::imshow("Histogram", histogramDisplay);
    cv::waitKey();

    return 0;
}

