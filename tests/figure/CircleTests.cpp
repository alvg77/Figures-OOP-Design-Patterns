#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>

#include "../../src/figure/circle/Circle.hpp"

constexpr double TOLERANCE = 1e-10;

struct CircleParams
{
    double radius, expected;
};

TEST_CASE("Valid circles produce correct perimeter calculation")
{
    auto [radius, expected] =
        GENERATE(values<CircleParams>({{5, 10.0 * M_PI}, {1e-5, 2e-5 * M_PI}, {10, 20.0 * M_PI}, {7.5, 15.0 * M_PI}}));

    CAPTURE(radius, expected);

    const Circle circle(radius);
    REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Circles handle extreme valid parameters")
{
    SECTION("Very large radius")
    {
        constexpr double large = std::numeric_limits<double>::max() / (2 * M_PI) - 1;
        const Circle circle(large);
        REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(2 * M_PI * large, TOLERANCE));
    }

    SECTION("Very small radius")
    {
        constexpr double small = std::numeric_limits<double>::min();
        const Circle circle(small);
        REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(2 * M_PI * small, TOLERANCE));
    }
}

TEST_CASE("clone creates independent copy")
{
    const Circle original(10);
    Circle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects invalid radius values")
{
    SECTION("Negative")
    {
        REQUIRE_THROWS_WITH(Circle(-1), "Radius must be a finite positive value");
    }

    SECTION("Zero")
    {
        REQUIRE_THROWS_WITH(Circle(0), "Radius must be a finite positive value");
    }

    SECTION("NaN")
    {
        REQUIRE_THROWS_WITH(Circle(std::numeric_limits<double>::quiet_NaN()), "Radius must be a finite positive value");
    }

    SECTION("Positive infinity")
    {
        REQUIRE_THROWS_WITH(Circle(std::numeric_limits<double>::infinity()), "Radius must be a finite positive value");
    }

    SECTION("Negative infinity")
    {
        REQUIRE_THROWS_WITH(Circle(-std::numeric_limits<double>::infinity()), "Radius must be a finite positive value");
    }
}

TEST_CASE("Constructor detects arithmetic overflow of perimeter")
{
    constexpr double large = std::numeric_limits<double>::max();
    REQUIRE_THROWS_WITH(Circle(large), "Perimeter must be a finite positive value");
}