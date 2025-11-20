#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../src/figure/triangle/Triangle.hpp"

constexpr double TOLERANCE = 1e-10;

struct TriangleParams
{
    double a, b, c;
};

TEST_CASE("Valid triangles produce correct perimeter calculation", "[Triangle]")
{
    auto [params, expected] = GENERATE(table<TriangleParams, double>(
        {{{3, 4, 5}, 12.0}, {{15, 25, 35}, 75.0}, {{7, 7, 7}, 21.0}, {{5, 5, 8}, 18.0}, {{0.1, 0.2, 0.25}, 0.55}}));

    CAPTURE(params.a, params.b, params.c, expected);

    const Triangle triangle(params.a, params.b, params.c);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Triangles handle extreme valid parameters", "[Triangle]")
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

TEST_CASE("Triangle clone creates independent copy", "[Triangle]")
{
    const Triangle original(5, 6, 7);
    Triangle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects negative and zero side lengths", "[Triangle]")
{
    auto [params, err] =
        GENERATE(table<TriangleParams, std::string>({{{-1, 2, 3}, "'a' must be a finite positive value"},
                                                     {{1, -2, 3}, "'b' must be a finite positive value"},
                                                     {{1, 2, -3}, "'c' must be a finite positive value"},
                                                     {{0, 2, 3}, "'a' must be a finite positive value"},
                                                     {{1, 0, 3}, "'b' must be a finite positive value"},
                                                     {{1, 2, 0}, "'c' must be a finite positive value"}}));

    CAPTURE(params.a, params.b, params.c);

    REQUIRE_THROWS_WITH(Triangle(params.a, params.b, params.c), err);
}

TEST_CASE("Constructor enforces triangle inequality", "[Triangle]")
{
    auto [a, b, c] = GENERATE(values<TriangleParams>({{1, 2, 10}, {1, 10, 2}, {10, 1, 2}, {1, 2, 3}}));

    CAPTURE(a, b, c);

    REQUIRE_THROWS_WITH(Triangle(a, b, c), "No triangle with such sides exist!");
}

TEST_CASE("Triangle constructor rejects NaN values", "[Triangle]")
{
    constexpr double nan = std::numeric_limits<double>::quiet_NaN();

    const int side = GENERATE(0, 1, 2);
    double sides[] = {1, 1, 1};
    const std::string errMessages[] = {"'a' must be a finite positive value", "'b' must be a finite positive value",
                                       "'c' must be a finite positive value"};
    sides[side] = nan;

    CAPTURE(side, sides);

    REQUIRE_THROWS_WITH(Triangle(sides[0], sides[1], sides[2]), errMessages[side]);
}

TEST_CASE("Triangle constructor rejects infinite values", "[Triangle]")
{
    auto [value, side] = GENERATE(table<double, unsigned>({{std::numeric_limits<double>::infinity(), 0},
                                                           {std::numeric_limits<double>::infinity(), 1},
                                                           {std::numeric_limits<double>::infinity(), 2},
                                                           {-std::numeric_limits<double>::infinity(), 0},
                                                           {-std::numeric_limits<double>::infinity(), 1},
                                                           {-std::numeric_limits<double>::infinity(), 2}}));

    std::array<double, 3> sides = {1, 1, 1};
    sides[side] = value;
    const std::string errMessages[] = {"'a' must be a finite positive value", "'b' must be a finite positive value",
                                       "'c' must be a finite positive value"};
    CAPTURE(side, sides);

    REQUIRE_THROWS_WITH(Triangle(sides[0], sides[1], sides[2]), errMessages[side]);
}

TEST_CASE("Triangle constructor detects arithmetic overflow of perimeter", "[Triangle]")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_WITH(Triangle(large, large, large), "Perimeter must be a finite positive value");
}