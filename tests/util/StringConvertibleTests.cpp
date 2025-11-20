#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "../../src/figure/triangle/Triangle.hpp"

struct TriangleToString
{
    double a, b, c;
    std::string expected;
};

struct RectangleToString
{
    double width, height;
    std::string expected;
};

struct CircleToString
{
    double radius;
    std::string expected;
};

TEST_CASE("Triangle to_string returns correct format", "[triangle][string]")
{
    auto [a, b, c, expected] =
        GENERATE(values<TriangleToString>({{3, 4, 5, "Triangle 3 4 5"},
                                           {3.5, 4.7, 5.9, "triangle 3.5 4.7 5.9"},
                                           {7, 7, 7, "Triangle 7 7 7"},
                                           {5, 5, 8, "Triangle 5 5 8"},
                                           {0.1, 0.2, 0.25, "Triangle 0.1 0.2 0.25"},
                                           {1.5e10, 2.5e10, 3.5e10, "Triangle 1.5e+10 2.5e+10 3.5e+10"},
                                           {0.003, 0.004, 0.005, "Triangle 0.003 0.004 0.005"}}));

    CAPTURE(a, b, c, expected);

    const Triangle triangle(a, b, c);
    const std::string result = triangle.toString();

    REQUIRE(result == expected);
}

TEST_CASE("to_string produces correct format for various dimensions", "[rectangle][string]")
{
    auto [width, height, expected] = GENERATE(values<RectangleToString>({{1, 2, "Rectangle 1 2"},
                                                                         {10, 20, "Rectangle 10 20"},
                                                                         {5.5, 7.3, "Rectangle 5.5 7.3"},
                                                                         {1.414, 2.718, "Rectangle 1.414 2.718"},
                                                                         {100, 100, "Rectangle 100 100"},
                                                                         {1e10, 2e10, "Rectangle 1e+10 2e+10"},
                                                                         {0.001, 0.002, "Rectangle 0.001 0.002"}}));

    CAPTURE(width, height, expected);

    const Rectangle rectangle(width, height);
    const std::string result = rectangle.toString();

    REQUIRE(result == expected);
}

TEST_CASE("to_string produces correct format for various radii", "[circle][string]")
{
    auto [radius, expected] = GENERATE(values<CircleToString>({{10, "Circle 10"},
                                                               {5, "Circle 5"},
                                                               {7.5, "Circle 7.5"},
                                                               {0.1, "Circle 0.1"},
                                                               {2.71828, "Circle 2.71828"},
                                                               {1.5e2, "Circle 1.5e2"},
                                                               {0.001, "Circle 0.001"}}));

    CAPTURE(radius, expected);

    const Circle circle(radius);
    const std::string result = circle.toString();

    REQUIRE(result == expected);
}