#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include "../../src/figure/rectangle/Rectangle.hpp"
#include "catch2/generators/catch_generators.hpp"

namespace
{
    constexpr double TOLERANCE = 1e-10;

    struct RectangleData
    {
        double width, height, expected;
    };

    struct RectangleToString
    {
        double width, height;
        std::string str;
    };

    struct InvalidData
    {
        double width, height;
        std::string problem;
    };
}

TEST_CASE("Valid rectangles produce correct perimeter calculation", "[rectangle][perimeter]")
{
    auto [width, height, expected] = GENERATE(values<RectangleData>({
        {10, 20, 60},
        {10, 5, 30.0},
        {12, 12, 48.0},
        {0.1, 0.2, 0.6}
    }));

    INFO("Could not get expected perimeter " << expected << " for values - width: " << width << ", height:" << height);

    const Rectangle rectangle(width, height);
    REQUIRE_THAT(rectangle.perimeter(), Catch::Matchers::WithinRel(expected, TOLERANCE));
}

TEST_CASE("Rectangles handle extreme valid parameters", "[rectangle][perimeter][edge]")
{
    SECTION("Very large sides") {
        const Rectangle rectangle(1e100, 1e100);
        REQUIRE_THAT(rectangle.perimeter(),
                     Catch::Matchers::WithinRel(4e100, TOLERANCE));
    }

    SECTION("Very small sides") {
        constexpr double small = std::numeric_limits<double>::min();
        const Rectangle rectangle(small, small);
        REQUIRE_THAT(rectangle.perimeter(),
                     Catch::Matchers::WithinRel(4 * small, TOLERANCE));
    }

    SECTION("Mixed magnitude sides") {
        const Rectangle rectangle(1e5, 1e-5);
        REQUIRE_THAT(rectangle.perimeter(),
                     Catch::Matchers::WithinRel(200000.00002, TOLERANCE));
    }
}

TEST_CASE("to_string produces correct format for various dimensions", "[rectangle][string]")
{
    auto [width, height, expected] = GENERATE(values<RectangleToString>({
        {1, 2, "Rectangle 1 2"},
        {10, 20, "Rectangle 10 20"},
        {5.5, 7.3, "Rectangle 5.5 7.3"},
        {0.1, 0.2, "Rectangle 0.1 0.2"},
        {100, 100, "Rectangle 100 100"},
        {1e10, 2e10, "Rectangle 1e+10 2e+10"},
        {0.001, 0.002, "Rectangle 0.001 0.002"}
    }));

    CAPTURE(width, height);

    const Rectangle rectangle(width, height);
    const std::string result = rectangle.to_string();

    REQUIRE(result == expected);
}

TEST_CASE("clone creates independent copy", "[rectangle][clone]")
{
    const Rectangle original(5, 10);
    Rectangle* cloned = original.clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != &original);
    REQUIRE_THAT(cloned->perimeter(),
                 Catch::Matchers::WithinRel(original.perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Constructor rejects negative dimensions", "[rectangle][validation][negative]")
{
    auto [width, height, desc] = GENERATE(values<InvalidData>({
        {-1, 1, "negative width"},
        {1, -1, "negative height"},
        {-1, -1, "both negative"}
    }));

    INFO("Testing with " << desc << ": (" << width << ", " << height << ")");

    REQUIRE_THROWS_AS(Rectangle(width, height), std::invalid_argument);
}

TEST_CASE("Constructor rejects zero dimensions", "[rectangle][validation]")
{
    auto [width, height, desc] = GENERATE(values<InvalidData>({
        {0, 1, "zero width"},
        {1, 0, "zero height"}
    }));

    INFO("Testing with " << desc << ": (" << width << ", " << height << ")");
    REQUIRE_THROWS_AS(Rectangle(width, height), std::invalid_argument);
}

TEST_CASE("Constructor rejects NaN values", "[rectangle][validation][special]")
{
    const auto side = GENERATE(0, 1);
    std::array<double, 2> dimensions = {1, 1};
    dimensions[side] = std::numeric_limits<double>::quiet_NaN();

    INFO("NaN in dimension " << side);
    REQUIRE_THROWS_AS(Rectangle(dimensions[0], dimensions[1]), std::invalid_argument);
}

TEST_CASE("Constructor rejects infinite values", "[rectangle][validation][special]")
{
    auto [value, side, desc] = GENERATE(table<double, int, const char*>({
        {std::numeric_limits<double>::infinity(), 0, "positive infinity in width"},
        {std::numeric_limits<double>::infinity(), 1, "positive infinity in height"},
        {-std::numeric_limits<double>::infinity(), 0, "negative infinity in width"},
        {-std::numeric_limits<double>::infinity(), 1, "negative infinity in height"}
    }));

    std::array<double, 2> dimensions = {1, 1};
    dimensions[side] = value;

    INFO("Testing " << desc);
    REQUIRE_THROWS_AS(Rectangle(dimensions[0], dimensions[1]), std::invalid_argument);
}

TEST_CASE("Constructor detects arithmetic overflow", "[rectangle][validation][overflow]")
{
    constexpr double large = std::numeric_limits<double>::max() / 2;
    REQUIRE_THROWS_AS(Rectangle(large, large), std::invalid_argument);
}