#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include "../../src/figure/rectangle/Rectangle.hpp"

TEST_CASE("Rectangle with width 10, height 20 has perimeter 60", "[rectangle][perimeter]")
{
    const Rectangle rectangle(10, 20);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(60.0, 1e-10));
}

TEST_CASE("Rectangle with width 10, height 5 has perimeter 30", "[rectangle][perimeter]")
{
    const Rectangle rectangle(10, 5);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(30.0, 1e-10));
}

TEST_CASE("Rectangle with equal width and height has correct perimeter", "[rectangle][perimeter]")
{
    const Rectangle rectangle(12, 12);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(48, 1e-10));
}

TEST_CASE("Rectangle with small decimal sides calculates perimeter correctly", "[rectangle][perimeter]")
{
    const Rectangle rectangle(.1, .2);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(.6, 1e-10));
}

TEST_CASE("Rectangle with large valid sides", "[rectangle][perimeter]")
{
    const Rectangle rectangle(1e100, 1e100);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(4e100, 1e-10));
}

TEST_CASE("Rectangle to_string returns correct format", "[rectangle][string]")
{
    const Rectangle rectangle(1, 2);
    std::string result = rectangle.to_string();
    REQUIRE(result.find("Rectangle") != std::string::npos);
    REQUIRE(result.find('1') != std::string::npos);
    REQUIRE(result.find('2') != std::string::npos);
}

TEST_CASE("Rectangle clone creates an interchangable copy", "[rectangle][clone]")
{
    const Rectangle original(5, 10);
    Rectangle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), 1e-10));

    delete cloned;
}

TEST_CASE("Rectangle clone creates independent copy", "[rectangle][clone]")
{
    const Rectangle original(5, 10);
    Rectangle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);

    delete cloned;
}

TEST_CASE("Rectangle construction fails with negative width", "[rectangle][invalid]")
{
    REQUIRE_THROWS_AS(Rectangle(-1, 1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails with negative height", "[rectangle][invalid]")
{
    REQUIRE_THROWS_AS(Rectangle(1, -1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails with all negative sides", "[rectangle][fails]")
{
    REQUIRE_THROWS_AS(Rectangle(-1, -1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for zero width", "[rectangle][fails]")
{
    REQUIRE_THROWS_AS(Rectangle(0, 1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for zero height", "[rectangle][fails]")
{
    REQUIRE_THROWS_AS(Rectangle(1, 0), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for NaN width", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(std::numeric_limits<double>::quiet_NaN(), 1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for NaN height", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(1, std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for positive infinity width", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(std::numeric_limits<double>::infinity(), 1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for positive infinity height", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(1, std::numeric_limits<double>::infinity()), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for negative infinity width", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(-std::numeric_limits<double>::infinity(), 1), std::invalid_argument);
}

TEST_CASE("Rectangle construction fails for negative infinity height", "[rectangle][special]")
{
    REQUIRE_THROWS_AS(Rectangle(1, -std::numeric_limits<double>::infinity()), std::invalid_argument);
}

TEST_CASE("Rectangle fails with parameters causing perimeter overflow", "[rectangle][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_AS(Rectangle(large, large), std::invalid_argument);
}

TEST_CASE("Rectangle with very small but valid parameters", "[rectangle][edge]")
{
    constexpr double small = std::numeric_limits<double>::min();
    const Rectangle rectangle(small, small);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(4 * small, 1e-10));

}

TEST_CASE("Rectangle with mixed magnitude parameters", "[rectangle][edge]")
{
    const Rectangle rectangle(1e5, 1e-5);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(200000.00002, 1e-10));
}