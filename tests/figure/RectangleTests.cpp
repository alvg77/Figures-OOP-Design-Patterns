#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../../src/figure/rectangle/Rectangle.hpp"

constexpr double TOLERANCE = 1e-10;

struct RectangleParams
{
    double width, height;
};

TEST_CASE("Valid rectangles produce correct perimeter calculation", "[Rectangle]")
{
    auto [params, expected] = GENERATE(
        table<RectangleParams, double>({{{10, 20}, 60}, {{10, 5}, 30.0}, {{12, 12}, 48.0}, {{0.1, 0.2}, 0.6}}));

    CAPTURE(params, expected);

    const Rectangle rectangle(params.width, params.height);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Rectangles handle extreme valid parameters", "[Rectangle]")
{
    SECTION("Very large sides")
    {
        const Rectangle rectangle(1e100, 1e100);
        REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(4e100, TOLERANCE));
    }

    SECTION("Very small sides")
    {
        constexpr double small = std::numeric_limits<double>::min();
        const Rectangle rectangle(small, small);
        REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(4 * small, TOLERANCE));
    }

    SECTION("Mixed magnitude sides")
    {
        const Rectangle rectangle(1e5, 1e-5);
        REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(200000.00002, TOLERANCE));
    }
}

TEST_CASE("Rectangle clone creates independent copy", "[Rectangle]")
{
    const Rectangle original(5, 10);
    Rectangle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Rectangle constructor rejects negative and zero dimensions", "[Rectangle]")
{
    auto [params, err] =
        GENERATE(table<RectangleParams, std::string>({{{-1, 1}, "'width' must be a finite positive value"},
                                                      {{1, -1}, "'height' must be a finite positive value"},
                                                      {{-1, -1}, "'width' must be a finite positive value"},
                                                      {{0, 1}, "'width' must be a finite positive value"},
                                                      {{1, 0}, "'height' must be a finite positive value"},
                                                      {{0, -1}, "'width' must be a finite positive value"}}));

    CAPTURE(params.width, params.height);

    REQUIRE_THROWS_WITH(Rectangle(params.width, params.height), err);
}

TEST_CASE("Rectangle constructor rejects NaN values", "[Rectangle]")
{
    const auto side = GENERATE(0, 1);
    double dimensions[] = {1, 1};
    const std::string errMessages[] = {"'width' must be a finite positive value",
                                       "'height' must be a finite positive value"};
    dimensions[side] = std::numeric_limits<double>::quiet_NaN();

    CAPTURE(side, dimensions);

    REQUIRE_THROWS_WITH(Rectangle(dimensions[0], dimensions[1]), errMessages[side]);
}

TEST_CASE("Rectangle constructor rejects infinite values", "[Rectangle]")
{
    auto [value, side] = GENERATE(table<double, unsigned>({{std::numeric_limits<double>::infinity(), 0},
                                                           {std::numeric_limits<double>::infinity(), 1},
                                                           {-std::numeric_limits<double>::infinity(), 0},
                                                           {-std::numeric_limits<double>::infinity(), 1}}));

    std::array<double, 2> dimensions = {1, 1};
    const std::string errMessages[] = {"'width' must be a finite positive value",
                                       "'height' must be a finite positive value"};
    dimensions[side] = value;

    CAPTURE(side, dimensions);

    REQUIRE_THROWS_WITH(Rectangle(dimensions[0], dimensions[1]), errMessages[side]);
}

TEST_CASE("Rectangle constructor detects arithmetic overflow of perimeter", "[Rectangle]")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_WITH(Rectangle(large, large), "Perimeter must be a finite positive value");
}