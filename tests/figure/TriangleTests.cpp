#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "../../src/figure/triangle/Triangle.hpp"


constexpr double TOLERANCE = 1e-10;

struct TriangleData
{
    double a, b, c, expected;
};

struct TriangleToString
{
    double a, b, c;
    std::string str;
};

struct InvalidData
{
    double a, b, c;
    const char *problem;
};

TEST_CASE("Valid triangles produce correct perimeter calculation", "[triangle][perimeter]")
{
    auto [a, b, c, expected] = GENERATE(values<TriangleData>(
        {{3, 4, 5, 12.0}, {15, 25, 35, 75.0}, {7, 7, 7, 21.0}, {5, 5, 8, 18.0}, {0.1, 0.2, 0.25, 0.55}}));

    CAPTURE(a, b, c, expected);

    const Triangle triangle(a, b, c);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Triangles handle extreme valid parameters", "[triangle][perimeter][edge]")
{
    SECTION("Very large sides")
    {
        const Triangle triangle(1e100, 1e100, 1e100);
        REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(3e100, TOLERANCE));
    }

    SECTION("Very small sides")
    {
        constexpr double small = std::numeric_limits<double>::min();
        const Triangle triangle(small, small, small);
        REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(3 * small, TOLERANCE));
    }

    SECTION("Mixed magnitude sides")
    {
        const Triangle triangle(1e5, 1e-5, 1e5 - 1e-51);
        REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(200000.00001, TOLERANCE));
    }

    SECTION("Near triangle inequality boundary")
    {
        const Triangle triangle(1.0, 1.0, 1.999);
        REQUIRE(triangle.perimeter() > 0);
    }
}

TEST_CASE("Triangle to_string returns correct format", "[triangle][string]")
{
    auto [a, b, c, expected] =
        GENERATE(values<TriangleToString>({{3, 4, 5, "Triangle 3 4 5"},
                                           {15, 25, 35, "Triangle 15 25 35"},
                                           {7, 7, 7, "Triangle 7 7 7"},
                                           {5, 5, 8, "Triangle 5 5 8"},
                                           {0.1, 0.2, 0.25, "Triangle 0.1 0.2 0.25"},
                                           {1.5e10, 2.5e10, 3.5e10, "Triangle 1.5e+10 2.5e+10 3.5e+10"},
                                           {0.003, 0.004, 0.005, "Triangle 0.003 0.004 0.005"}}));

    CAPTURE(a, b, c);

    const Triangle triangle(a, b, c);
    const std::string result = triangle.to_string();

    REQUIRE(result == expected);
}

TEST_CASE("clone creates independent copy", "[triangle][clone]")
{
    const Triangle original(5, 6, 7);
    Triangle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects negative side lengths", "[triangle][validation][negative]")
{
    auto [a, b, c, problem] = GENERATE(
        values<InvalidData>({{-1, 2, 3, "a is negative"}, {1, -2, 3, "b is negative"}, {1, 2, -3, "c is negative"}}));

    INFO("Test fails at " << problem << ": (" << a << ", " << b << ", " << c << ")");

    REQUIRE_THROWS_AS(Triangle(a, b, c), std::invalid_argument);
}

TEST_CASE("Constructor rejects zero side lengths", "[triangle][validation][zero]")
{
    auto [a, b, c, problem] =
        GENERATE(values<InvalidData>({{0, 2, 3, "a is zero"}, {1, 0, 3, "b is zero"}, {1, 2, 0, "c is zero"}}));

    INFO("Test fails at" << problem << ": (" << a << ", " << b << ", " << c << ")");
    REQUIRE_THROWS_AS(Triangle(a, b, c), std::invalid_argument);
}

TEST_CASE("Constructor enforces triangle inequality", "[triangle][validation][invalid side constraints]")
{
    auto [a, b, c, problem] = GENERATE(values<InvalidData>({{1, 2, 10, "a + b < c"},
                                                            {1, 10, 2, "a + c < b"},
                                                            {10, 1, 2, "b + c < a"},
                                                            {1, 2, 3, "degenerate: a + b = c"}}));

    INFO("Triangle inequality violation: " << problem);

    REQUIRE_THROWS_AS(Triangle(a, b, c), std::invalid_argument);
}

TEST_CASE("Constructor rejects NaN values", "[triangle][validation][nan]")
{
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();

    const int side = GENERATE(0, 1, 2);
    std::array<double, 3> sides = {1, 1, 1};
    sides[side] = nan;

    INFO("NaN in position " << side);

    REQUIRE_THROWS_AS(Triangle(sides[0], sides[1], sides[2]), std::invalid_argument);
}

TEST_CASE("Constructor rejects infinite values", "[triangle][validation][infinity]")
{
    auto [value, side, problem] = GENERATE(
        table<double, int, const char *>({{std::numeric_limits<double>::infinity(), 0, "positive infinity in a"},
                                          {std::numeric_limits<double>::infinity(), 1, "positive infinity in b"},
                                          {std::numeric_limits<double>::infinity(), 2, "positive infinity in c"},
                                          {-std::numeric_limits<double>::infinity(), 0, "negative infinity in a"},
                                          {-std::numeric_limits<double>::infinity(), 1, "negative infinity in b"},
                                          {-std::numeric_limits<double>::infinity(), 2, "negative infinity in c"}}));

    std::array<double, 3> sides = {1, 1, 1};
    sides[side] = value;

    INFO("Testing " << problem);

    REQUIRE_THROWS_AS(Triangle(sides[0], sides[1], sides[2]), std::invalid_argument);
}

TEST_CASE("Constructor detects arithmetic overflow", "[triangle][validation][overflow]")
{
    SECTION("Three sides causing overflow")
    {
        constexpr double large = std::numeric_limits<double>::max() / 2;
        REQUIRE_THROWS_AS(Triangle(large, large, large), std::invalid_argument);
    }

    SECTION("Two sides causing overflow")
    {
        constexpr double max_val = std::numeric_limits<double>::max();
        REQUIRE_THROWS_AS(Triangle(max_val, max_val, 1), std::invalid_argument);
    }
}