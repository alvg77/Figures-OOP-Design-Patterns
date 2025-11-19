#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <functional>

#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"
#include "../../src/figure/triangle/Triangle.hpp"
#include "../../src/util/string_to_figure/StringToFigure.hpp"

TEST_CASE("StringToFigure::create_figure creates valid Triangle", "[StringToFigure]")
{
    SECTION("lowercase triangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("uppercase TRIANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("TRIANGLE 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("mixed case TrIaNgLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("TrIaNgLe 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("triangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3 4 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("triangle with extra whitespace")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle  3.0   4.0    5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("StringToFigure::create_figure creates valid Circle", "[StringToFigure]")
{
    SECTION("lowercase circle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("uppercase CIRCLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("CIRCLE 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("mixed case CiRcLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("CiRcLe 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("circle with integer parameter")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("StringToFigure::create_figure creates valid Rectangle", "[StringToFigure]")
{
    SECTION("lowercase rectangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("rectangle 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("uppercase RECTANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("RECTANGLE 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("mixed case ReCtAnGlE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("ReCtAnGlE 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("rectangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("rectangle 4 6");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("StringToFigure::create_figure throws on invalid parameter count", "[StringToFigure]")
{
    SECTION("triangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("triangle 3.0 4.0"), "triangle requires three parameters");
    }

    SECTION("triangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 4.0 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("triangle 3.0 4.0 5.0 6.0"),
                            "triangle requires three parameters");
    }

    SECTION("circle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("circle"), "circle requires one parameter");
    }

    SECTION("circle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("circle 5.0 6.0"), "circle requires one parameter");
    }

    SECTION("rectangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("rectangle 4.0"), "rectangle requires two parameters");
    }

    SECTION("rectangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0 6.0 8.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("rectangle 4.0 6.0 8.0"),
                            "rectangle requires two parameters");
    }
}

TEST_CASE("StringToFigure::create_figure throws on invalid figure name", "[StringToFigure]")
{
    SECTION("unknown figure name")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("square 5.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("square 5.0"), "Invalid figure string");
    }

    SECTION("empty string")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure(""), std::invalid_argument);
    }

    SECTION("only whitespace")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("   "), std::invalid_argument);
    }

    SECTION("gibberish")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("asdfghjkl 1 2 3"), std::invalid_argument);
    }
}

TEST_CASE("StringToFigure::create_figure throws on invalid numeric parameters", "[StringToFigure]")
{
    SECTION("non-numeric parameter for triangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 abc 5.0"), std::invalid_argument);
    }

    SECTION("non-numeric parameter for circle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle xyz"), std::invalid_argument);
    }

    SECTION("non-numeric parameter for rectangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0 notanumber"), std::invalid_argument);
    }
}

TEST_CASE("StringToFigure::create_figure handles negative and zero values", "[StringToFigure]")
{
    SECTION("negative parameter for circle")
    {
        auto result = []() { return StringToFigure::create_figure("circle -5.0"); };
        REQUIRE_THROWS_AS(result(), std::invalid_argument);
    }

    SECTION("zero parameter for rectangle")
    {
        auto result = []() { return StringToFigure::create_figure("rectangle 0.0 5.0"); };
        REQUIRE_THROWS_AS(result(), std::invalid_argument);
    }
}

TEST_CASE("StringToFigure::create_figure handles scientific notation", "[StringToFigure]")
{
    SECTION("circle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 1.5e2");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("triangle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3e0 4e0 5e0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("StringToFigure::is_double validates numeric strings", "[StringToFigure]")
{
    SECTION("valid doubles")
    {
        REQUIRE(StringToFigure::is_double("3.14"));
        REQUIRE(StringToFigure::is_double("42"));
        REQUIRE(StringToFigure::is_double("-5.5"));
        REQUIRE(StringToFigure::is_double("0.0"));
        REQUIRE(StringToFigure::is_double("1e10"));
        REQUIRE(StringToFigure::is_double("1.5e-3"));
        REQUIRE(StringToFigure::is_double(".5"));
        REQUIRE(StringToFigure::is_double("5."));
    }

    SECTION("invalid doubles")
    {
        REQUIRE_FALSE(StringToFigure::is_double("abc"));
        REQUIRE_FALSE(StringToFigure::is_double("3.14.15"));
        REQUIRE_FALSE(StringToFigure::is_double(""));
        REQUIRE_FALSE(StringToFigure::is_double("   "));
        REQUIRE_FALSE(StringToFigure::is_double("12abc"));
        REQUIRE_FALSE(StringToFigure::is_double("abc12"));
        REQUIRE_FALSE(StringToFigure::is_double("NaN"));
        REQUIRE_FALSE(StringToFigure::is_double("infinity"));
    }

    SECTION("edge cases")
    {
        REQUIRE(StringToFigure::is_double("inf"));
        REQUIRE(StringToFigure::is_double("-inf"));
        REQUIRE(StringToFigure::is_double("nan"));
    }
}

TEST_CASE("StringToFigure::create_figure handles decimal values", "[StringToFigure]")
{
    SECTION("triangle with decimal values")
    {
        auto figure = StringToFigure::create_figure("triangle 3.5 4.7 5.9");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("circle with decimal value")
    {
        auto figure = StringToFigure::create_figure("circle 2.71828");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("rectangle with decimal values")
    {
        auto figure = StringToFigure::create_figure("rectangle 1.414 2.718");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}
