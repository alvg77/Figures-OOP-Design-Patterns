#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include "../../src/figure/triangle/Triangle.hpp"

TEST_CASE("Triangle with sides 15, 25, 35 has perimeter 75", "[triangle][perimeter]")
{
    const Triangle triangle(15, 25, 35);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(75.0, 1e-10));
}

TEST_CASE("Triangle with sides 3, 4, 5 has perimeter 12", "[triangle][perimeter]")
{
    const Triangle triangle(3, 4, 5);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(12.0, 1e-10));
}

TEST_CASE("Triangle with equal sides has correct perimeter", "[triangle][perimeter]")
{
    const Triangle triangle(7, 7, 7);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(21.0, 1e-10));
}

TEST_CASE("Triangle with small decimal sides calculates perimeter correctly", "[triangle][perimeter]")
{
    const Triangle triangle(.1, .2, .25);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(.55, 1e-10));
}

TEST_CASE("Triangle with large valid sides", "[triangle][perimeter]")
{
    const Triangle triangle(1e100, 1e100, 1e100);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(3e100, 1e-10));
}

TEST_CASE("Triangle to_string returns correct format", "[triangle][string]")
{
    const Triangle triangle(1, 3, 3);
    std::string result = triangle.to_string();
    REQUIRE(result.find("Triangle") != std::string::npos);
    REQUIRE(result.find('1') != std::string::npos);
    REQUIRE(result.find('3') != std::string::npos);
    REQUIRE(result.find('3') != std::string::npos);
}

TEST_CASE("Triangle clone creates identical copy", "[triangle][clone]")
{
    const Triangle original(5, 6, 7);
    Triangle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), 1e-10));

    delete cloned;
}

TEST_CASE("Triangle clone creates independent copy", "[triangle][clone]")
{
    const Triangle original(5, 6, 7);
    Triangle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);

    delete cloned;
}

TEST_CASE("Triangle construction fails with negative side a", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(-1, 2, 3), std::invalid_argument);
}

TEST_CASE("Triangle construction fails with negative side b", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(1, -2, 3), std::invalid_argument);
}

TEST_CASE("Triangle construction fails with negative side c", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(1, 2, -3), std::invalid_argument);
}

TEST_CASE("Triangle construction fails with zero side a", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(0, 2, 3), std::invalid_argument);
}

TEST_CASE("Triangle construction fails with zero side b", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(1, 0, 3), std::invalid_argument);
}

TEST_CASE("Triangle construction fails with zero side c", "[triangle][invalid]")
{
    REQUIRE_THROWS_AS(Triangle(1, 2, 0), std::invalid_argument);
}

TEST_CASE("Triangle fails when a + b < c", "[triangle][inequality]")
{
    REQUIRE_THROWS_AS(Triangle(1, 2, 10), std::invalid_argument);
}

TEST_CASE("Triangle fails when a + c < b", "[triangle][inequality]")
{
    REQUIRE_THROWS_AS(Triangle(1, 10, 2), std::invalid_argument);
}

TEST_CASE("Triangle fails when b + c < a", "[triangle][inequality]")
{
    REQUIRE_THROWS_AS(Triangle(10, 1, 2), std::invalid_argument);
}

TEST_CASE("Triangle fails when a + b == c exactly (degenerate)", "[triangle][inequality]")
{
    REQUIRE_THROWS_AS(Triangle(1, 2, 3), std::invalid_argument);
}

TEST_CASE("Triangle fails with NaN in side a", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(std::numeric_limits<double>::quiet_NaN(), 1, 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with NaN in side b", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, std::numeric_limits<double>::quiet_NaN(), 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with NaN in side c", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, 1, std::numeric_limits<double>::quiet_NaN()),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with positive infinity in side a", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(std::numeric_limits<double>::infinity(), 1, 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with positive infinity in side b", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, std::numeric_limits<double>::infinity(), 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with positive infinity in side c", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, 1, std::numeric_limits<double>::infinity()),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with negative infinity in side a", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(-std::numeric_limits<double>::infinity(), 1, 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with negative infinity in side b", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, -std::numeric_limits<double>::infinity(), 1),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails with negative infinity in side c", "[triangle][special]")
{
    REQUIRE_THROWS_AS(Triangle(1, 1, -std::numeric_limits<double>::infinity()),
                      std::invalid_argument);
}

TEST_CASE("Triangle fails when perimeter would overflow to infinity", "[triangle][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_AS(Triangle(large, large, large), std::invalid_argument);
}

TEST_CASE("Triangle fails with two very large sides that overflow", "[triangle][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max();
    REQUIRE_THROWS_AS(Triangle(large, large, 1), std::invalid_argument);
}

TEST_CASE("Triangle with very small but valid sides", "[triangle][edge]")
{
    constexpr double small = std::numeric_limits<double>::min();
    const Triangle triangle(small, small, small);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(3 * small, 1e-10));
}

TEST_CASE("Triangle with mixed size sides", "[triangle][edge]")
{
    const Triangle triangle(1e5, 1e-5, 1e5 - 1e-51);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(200000.00001, 1e-10));
}

TEST_CASE("Triangle with near-equality in triangle inequality", "[triangle][edge]")
{
    const Triangle triangle(1.0, 1.0, 1.999);
    REQUIRE(triangle.perimeter() > 0);
}

TEST_CASE("Isosceles triangle perimeter", "[triangle][types]")
{
    const Triangle triangle(5, 5, 8);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(18.0, 1e-10));
}

TEST_CASE("Right triangle perimeter", "[triangle][types]")
{
    const Triangle triangle(3, 4, 5);
    REQUIRE_THAT(triangle.perimeter(), Catch::Matchers::WithinRel(12.0, 1e-10));
}
