#include <boost/test/unit_test.hpp>

#include <opencv2/core.hpp>

#include "utils_impl.hpp"
#include "Processing_impl.hpp"

template<typename T>
void checkEqual(const cv::Mat& m1, const cv::Mat& m2)
{
    BOOST_REQUIRE_EQUAL(m1.size(), m2.size());
    BOOST_REQUIRE_EQUAL(m1.type(), m2.type());

    const std::vector<T> t1(m1.ptr<T>(0), m1.ptr<T>(0) + m1.total() * m1.channels());
    const std::vector<T> t2(m2.ptr<T>(0), m2.ptr<T>(0) + m2.total() * m2.channels());

    for (unsigned long i = 0; i < t1.size(); i++)
        BOOST_CHECK_EQUAL(t1[i], t2[i]);
}

BOOST_AUTO_TEST_SUITE(utils)

BOOST_AUTO_TEST_CASE(testSplit)
{
    const cv::Mat toSplit(3, 3, CV_32FC4);
    cv::randu(toSplit, -10, 10);

    std::vector<cv::Mat> splitted, splittedCV;
    cv::split(toSplit, splittedCV);
    utils::split(toSplit, splitted);

    BOOST_REQUIRE_EQUAL(splitted.size(), splittedCV.size());

    for (unsigned int i = 0; i < splitted.size(); i++)
        checkEqual<float>(splitted[i], splittedCV[i]);
}

BOOST_AUTO_TEST_CASE(testMerge)
{
    std::vector<cv::Mat> toMerge;
    cv::Mat merged, mergedCV;
    BOOST_CHECK_THROW(utils::merge(toMerge, merged), cv::Exception); // There cannot be no matrixes to merge
    toMerge = { cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat() };
    BOOST_CHECK_THROW(utils::merge(toMerge, merged), cv::Exception); // There cannot be more than 4 matrixes to merge
    toMerge.pop_back();
    BOOST_CHECK_NO_THROW(utils::merge(toMerge, merged));

    toMerge = {cv::Mat(5, 5, CV_8UC1), cv::Mat(5, 5, CV_8UC2) };
    BOOST_CHECK_THROW(utils::merge(toMerge, merged), cv::Exception); // Matrixes must all have 1 channels
    toMerge = {cv::Mat(5, 5, CV_8UC1), cv::Mat(6, 6, CV_8UC1) };
    BOOST_CHECK_THROW(utils::merge(toMerge, merged), cv::Exception); // Matrixes must all have the same size

    cv::Mat merge1(5, 5, CV_32FC1), merge3(5, 5, CV_32FC1), merge2(5, 5, CV_32FC1);
    cv::randu(merge1, -10, 10);
    cv::randu(merge2, -10, 10);
    cv::randu(merge3, -10, 10);
    toMerge = { merge1, merge2, merge3 };

    cv::merge(toMerge, mergedCV);
    utils::merge(toMerge, merged);

    checkEqual<float>(merged, mergedCV);
}

BOOST_AUTO_TEST_CASE(testGetMinMaxValue)
{
    cv::Mat m(3, 3, CV_32SC3);
    cv::randu(m, -10, 10);
    m.at<cv::Vec3i>(0, 1) = { 1, -50, 3}; // Set min = -50 at col 1, row 0 chan 1
    m.at<cv::Vec3i>(2, 1) = { 1, 2, 50 }; // Set max = 50 at col 1, row 2, chan 2

    const cv::Mat minMax = utils::getMinMaxValue(m);
    BOOST_CHECK_EQUAL(minMax.at<int>(0, 0), -50);
    BOOST_CHECK_EQUAL(minMax.at<int>(0, 1), 1);
    BOOST_CHECK_EQUAL(minMax.at<int>(0, 2), 0);
    BOOST_CHECK_EQUAL(minMax.at<int>(0, 3), 1);

    BOOST_CHECK_EQUAL(minMax.at<int>(1, 0), 50);
    BOOST_CHECK_EQUAL(minMax.at<int>(1, 1), 1);
    BOOST_CHECK_EQUAL(minMax.at<int>(1, 2), 2);
    BOOST_CHECK_EQUAL(minMax.at<int>(1, 3), 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(histograms)

BOOST_AUTO_TEST_CASE(testHistogram)
{
    cv::Mat input(1, 1, CV_8S);
    BOOST_CHECK_THROW(Processing::impl::computeImageHistogram(input), cv::Exception); // Depth 8U only
    input.create(1, 1, CV_8UC2);
    BOOST_CHECK_THROW(Processing::impl::computeImageHistogram(input), cv::Exception); // 1, 3 or 4 channels only
    input.create(1, 1, CV_8UC4);
    BOOST_CHECK_NO_THROW(Processing::impl::computeImageHistogram(input)); // ok

    input.create(3, 3, CV_8UC4);
    cv::randu(input, 0, 10);
    const cv::Mat multiChannelHistogram = Processing::impl::computeImageHistogram(input);
    BOOST_CHECK_EQUAL(multiChannelHistogram.depth(), CV_32S); // Depth of an histogram always is int (32S)
    BOOST_CHECK_EQUAL(input.channels(), multiChannelHistogram.channels()); // For multi channels input, compute one histogram per channel

    input = (cv::Mat_<uchar>(4, 4) << 0, 1, 2, 2,
             3, 3, 3, 4,
             4, 4, 4, 5,
             5, 5, 5, 5);

    cv::Mat groundTruthHistogram = cv::Mat::zeros(1, 256, CV_32SC1);
    groundTruthHistogram.at<int>(0, 0) = 1;
    groundTruthHistogram.at<int>(0, 1) = 1;
    groundTruthHistogram.at<int>(0, 2) = 2;
    groundTruthHistogram.at<int>(0, 3) = 3;
    groundTruthHistogram.at<int>(0, 4) = 4;
    groundTruthHistogram.at<int>(0, 5) = 5;

    const cv::Mat histogram = Processing::impl::computeImageHistogram(input);
    checkEqual<int>(histogram, groundTruthHistogram);
}

BOOST_AUTO_TEST_CASE(testRowHistogram)
{
    const int rowIndex = 20;
    cv::Mat input(50, 50, CV_8UC3);
    cv::randu(input, 0, 255);
    cv::Mat inputRow = input.row(rowIndex);

    BOOST_CHECK_THROW(Processing::impl::computeImageRowHistogram(input, -12), cv::Exception);
    BOOST_CHECK_THROW(Processing::impl::computeImageRowHistogram(input, 100), cv::Exception);

    checkEqual<int>(Processing::impl::computeImageRowHistogram(input, rowIndex), Processing::impl::computeImageHistogram(inputRow));
}

BOOST_AUTO_TEST_CASE(testColumnHistogram)
{
    const int colIndex = 20;
    cv::Mat input(50, 50, CV_8UC3);
    cv::randu(input, 0, 255);
    cv::Mat inputColumn = input.t().row(colIndex);

    BOOST_CHECK_THROW(Processing::impl::computeImageColumnHistogram(input, -12), cv::Exception);
    BOOST_CHECK_THROW(Processing::impl::computeImageColumnHistogram(input, 100), cv::Exception);

    checkEqual<int>(Processing::impl::computeImageColumnHistogram(input, colIndex), Processing::impl::computeImageHistogram(inputColumn));
}

BOOST_AUTO_TEST_CASE(testCumulativeHistogram)
{
    cv::Mat input = (cv::Mat_<uchar>(4, 4) << 0, 1, 2, 2,
                     3, 3, 3, 4,
                     4, 4, 4, 5,
                     5, 5, 5, 5);

    cv::Mat groundTruthCumulativeHistogram(1, 256, CV_32SC1, 16);
    groundTruthCumulativeHistogram.at<int>(0, 0) = 1;
    groundTruthCumulativeHistogram.at<int>(0, 1) = 2;
    groundTruthCumulativeHistogram.at<int>(0, 2) = 4;
    groundTruthCumulativeHistogram.at<int>(0, 3) = 7;
    groundTruthCumulativeHistogram.at<int>(0, 4) = 11;
    groundTruthCumulativeHistogram.at<int>(0, 5) = 16;

    const cv::Mat cumulativeHistogram = Processing::impl::computeImageCumulativeHistogram(input);
    checkEqual<int>(cumulativeHistogram, groundTruthCumulativeHistogram);
}

BOOST_AUTO_TEST_SUITE_END()
