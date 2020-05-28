#ifndef UTILS_TPP
#define UTILS_TPP

#include "utils.hpp"

namespace utils
{

template<class T>
void split(const cv::Mat& input, std::vector<cv::Mat>& splittedInput)
{
    splittedInput.clear();
    const T* inputDataPtr = input.template ptr<T>(0);
    std::vector<T*> splittedDataPtrs;
    for (uchar c = 0; c < input.channels(); c++)
    {
        splittedInput.push_back(cv::Mat::zeros(input.size(), input.depth()));
        splittedDataPtrs.push_back(splittedInput[c].template ptr<T>(0));
    }
    for (unsigned long i = 0; i < input.total(); i++)
        for (uchar c = 0; c < input.channels(); c++)
            splittedDataPtrs[c][i] = inputDataPtr[i * input.channels() + c];
}

template<class T>
void merge(const std::vector<cv::Mat>& inputs, cv::Mat& mergedInputs)
{
    CV_Assert(inputs.size() > 0 && inputs.size() <= 4);

    std::vector<const T*> inputsDataPtrs;
    for (uchar i = 0; i < inputs.size(); i++)
    {
        CV_Assert(inputs[i].channels() == 1);
        CV_Assert(inputs[i].size() == inputs[0].size());
        inputsDataPtrs.push_back(inputs[i].template ptr<T>(0));
    }

    mergedInputs.create(inputs[0].size(), (inputs.size() - 1) * 8 + inputs[0].depth());
    T* mergedInputDataPtr = mergedInputs.template ptr<T>(0);

    for (unsigned int i = 0; i < mergedInputs.total(); i++)
        for (uchar c = 0; c < mergedInputs.channels(); c++)
            mergedInputDataPtr[i * mergedInputs.channels() + c] = inputsDataPtrs[c][i];
}

template<class T>
cv::Mat getMinMaxValue(const cv::Mat& input)
{
    cv::Mat minMax(2, 4, input.depth(), cv::Scalar::all(0));

    T* minPtr = minMax.ptr<T>(0);
    T* maxPtr = minMax.ptr<T>(1);
    minPtr[0] = std::numeric_limits<T>::max();
    maxPtr[0] = std::numeric_limits<T>::min();

    for (int i = 0; i < input.rows; i++)
    {
        const T* rowPtr = input.ptr<T>(i);
        for (int j = 0; j < input.cols; j++)
        {
            for (int c = 0; c < input.channels(); c++)
            {
                if (rowPtr[j * input.channels() + c] < minPtr[0])
                {
                    minPtr[0] = rowPtr[j * input.channels() + c];
                    minPtr[1] = j;
                    minPtr[2] = i;
                    minPtr[3] = c;
                }

                if (rowPtr[j * input.channels() + c] > maxPtr[0])
                {
                    maxPtr[0] = rowPtr[j * input.channels() + c];
                    maxPtr[1] = j;
                    maxPtr[2] = i;
                    maxPtr[3] = c;
                }
            }
        }
    }

    return minMax;
}

}

#endif //UTILS_TPP
