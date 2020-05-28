#include "Processing.hpp"
#include "Display.hpp"

#include <opencv2/highgui.hpp>

int main(int, char** argv)
{
    const cv::Mat input = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
    if (input.empty())
        return -1;

    cv::Mat colorConvertedInput;
    Processing::convertImageColor(input, colorConvertedInput, Processing::Conversion::BGR2Grey_Lightness);

    cv::imshow("Input", input);
    cv::imshow("Grey input", colorConvertedInput);
    cv::waitKey();

    return 0;
}
