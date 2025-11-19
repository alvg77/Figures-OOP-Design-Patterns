#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <cmath>

#include "../../src/figure/circle/Circle.hpp"


constexpr double TOLERANCE = 1e-10;

struct CircleData
{
    double radius, expected;
};

struct CircleToString
{
    double radius;
    std::string str;
};

TEST_CASE("Valid circles produce correct perimeter calculation", "[circle][perimeter]")
{
    auto [radius, expected] =
        GENERATE(values<CircleData>({{5, 10.0 * M_PI}, {1e-5, 2e-5 * M_PI}, {10, 20.0 * M_PI}, {7.5, 15.0 * M_PI}}));

    CAPTURE(radius, expected);

    const Circle circle(radius);
    REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Circles handle extreme valid parameters", "[circle][perimeter][edge]")
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

TEST_CASE("to_string produces correct format for various radii", "[circle][string]")
{
    auto [radius, expected] = GENERATE(values<CircleToString>({{10, "Circle 10"},
                                                               {5, "Circle 5"},
                                                               {7.5, "Circle 7.5"},
                                                               {0.1, "Circle 0.1"},
                                                               {100, "Circle 100"},
                                                               {1e10, "Circle 1e+10"},
                                                               {0.001, "Circle 0.001"}}));

    CAPTURE(radius);

    const Circle circle(radius);
    const std::string result = circle.to_string();

    REQUIRE(result == expected);
}

TEST_CASE("clone creates independent copy", "[circle][clone]")
{
    const Circle original(10);
    Circle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects negative radius", "[circle][validation][negative]")
{
    REQUIRE_THROWS_AS(Circle(-1), std::invalid_argument);
}

TEST_CASE("Constructor rejects zero radius", "[circle][validation]")
{
    REQUIRE_THROWS_AS(Circle(0), std::invalid_argument);
}

TEST_CASE("Constructor rejects NaN value", "[circle][validation][special]")
{
    REQUIRE_THROWS_AS(Circle(std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
}

TEST_CASE("Constructor rejects infinite values", "[circle][validation][special]")
{
    SECTION("Positive infinity")
    {
        REQUIRE_THROWS_AS(Circle(std::numeric_limits<double>::infinity()), std::invalid_argument);
    }

    SECTION("Negative infinity")
    {
        REQUIRE_THROWS_AS(Circle(-std::numeric_limits<double>::infinity()), std::invalid_argument);
    }
}

TEST_CASE("Constructor detects arithmetic overflow", "[circle][validation][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max();
    REQUIRE_THROWS_AS(Circle(large), std::invalid_argument);
}