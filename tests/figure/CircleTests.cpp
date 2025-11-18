#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include "../../src/figure/circle/Circle.hpp"

TEST_CASE("Circle with radius 5 and parameter 10 * pi", "[circle][perimeter]")
{
    const Circle circle(5);
    REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(10.0*M_PI, 1e-10));
}

TEST_CASE("Circle with small decimal radius calculates perimeter correctly", "[circle][perimeter]")
{
    const Circle circle(1e-5);
    REQUIRE_THAT(circle.perimeter(), Catch::Matchers::WithinRel(M_PI * 2e-5, 1e-10));
}

TEST_CASE("Circle to_string returns correct format", "[circle][string]")
{
    const Circle circle(10);
    std::string result = circle.to_string();
    REQUIRE(result.find("Circle") != std::string::npos);
    REQUIRE(result.find("10") != std::string::npos);
}

TEST_CASE("Circle clone creates identical copy", "[circle][clone]")
{
    const Circle original(10);
    Circle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), 1e-10));

    delete cloned;
}

TEST_CASE("Circle clone creates independent copy", "[circle][clone]")
{
    const Circle original(10);
    Circle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);

    delete cloned;
}

TEST_CASE("Circle fails with negative radius", "[circle][invalid]")
{
    REQUIRE_THROWS_AS(Circle(-1), std::invalid_argument);
}

TEST_CASE("Circle fails with zero radius", "[circle][invalid]")
{
    REQUIRE_THROWS_AS(Circle(0), std::invalid_argument);
}

TEST_CASE("Circle fails with NaN radius", "[circle][special]")
{
    REQUIRE_THROWS_AS(Circle(std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
}

TEST_CASE("Circle fails with positive infinity radius", "[circle][special]")
{
    REQUIRE_THROWS_AS(Circle(std::numeric_limits<double>::infinity()), std::invalid_argument);
}

TEST_CASE("Circle fails with negative infinity radius", "[circle][special]")
{
    REQUIRE_THROWS_AS(Circle(-std::numeric_limits<double>::infinity()), std::invalid_argument);
}

TEST_CASE("Circle fails when perimeter would overflow to infinity", "[circle][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max();
    REQUIRE_THROWS_AS(Circle(large), std::invalid_argument);
}

TEST_CASE("Circle with very small but valid radius", "[circle][edge]")
{
    constexpr double small = std::numeric_limits<double>::min();
    const Circle triangle(small);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(2 * M_PI * small, 1e-10));
}

