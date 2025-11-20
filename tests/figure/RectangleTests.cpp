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

TEST_CASE("Valid rectangles produce correct perimeter calculation")
{
    auto [params, expected] = GENERATE(
        table<RectangleParams, double>({{{10, 20}, 60}, {{10, 5}, 30.0}, {{12, 12}, 48.0}, {{0.1, 0.2}, 0.6}}));

    CAPTURE(params, expected);

    const Rectangle rectangle(params.width, params.height);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Rectangles handle extreme valid parameters")
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

TEST_CASE("clone creates independent copy")
{
    const Rectangle original(5, 10);
    Rectangle *cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects negative dimensions")
{
    auto [width, height] = GENERATE(values<RectangleParams>({{-1, 1}, {1, -1}, {-1, -1}}));

    CAPTURE(width, height);

    REQUIRE_THROWS_AS(Rectangle(width, height), std::invalid_argument);
}

TEST_CASE("Constructor rejects zero dimensions", "[rectangle][validation][zero]")
{
    auto [width, height] = GENERATE(values<RectangleParams>({{0, 1}, {1, 0}}));

    CAPTURE(width, height);

    REQUIRE_THROWS_AS(Rectangle(width, height), std::invalid_argument);
}

TEST_CASE("Constructor rejects NaN values", "[rectangle][validation][nan]")
{
    const auto side = GENERATE(0, 1);
    std::array<double, 2> dimensions = {1, 1};
    dimensions[side] = std::numeric_limits<double>::quiet_NaN();

    CAPTURE(side, dimensions);

    REQUIRE_THROWS_AS(Rectangle(dimensions[0], dimensions[1]), std::invalid_argument);
}

TEST_CASE("Constructor rejects infinite values", "[rectangle][validation][infinity]")
{
    auto [value, side] = GENERATE(table<double, unsigned>({{std::numeric_limits<double>::infinity(), 0},
                                                           {std::numeric_limits<double>::infinity(), 1},
                                                           {-std::numeric_limits<double>::infinity(), 0},
                                                           {-std::numeric_limits<double>::infinity(), 1}}));

    std::array<double, 2> dimensions = {1, 1};
    dimensions[side] = value;

    CAPTURE(side, dimensions);

    REQUIRE_THROWS_AS(Rectangle(dimensions[0], dimensions[1]), std::invalid_argument);
}

TEST_CASE("Constructor detects arithmetic overflow")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_AS(Rectangle(large, large), std::invalid_argument);
}