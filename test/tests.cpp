#include <boost/test/unit_test.hpp>

#include <opencv2/core.hpp>

#include "utils_impl.hpp"

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
