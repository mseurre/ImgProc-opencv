#ifndef ENUM_HPP
#define ENUM_HPP

namespace Processing
{

enum class Conversion
{
    BGR2Grey_Average,
    BGR2Grey_Lightness,
    BGR2Grey_Luminosity,
    Grey2BGR
};

enum class Filter
{
    Mean,
    Median
};

enum class EdgeDetection
{
    Laplacian,
    Sobel
};

enum class Histogram
{
    Normal,
    Row,
    Column,
    Cumulative
};

}

namespace Display
{

enum class Histogram
{
    Curve,
    Bar
};

}

#endif
