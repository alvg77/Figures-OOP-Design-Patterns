#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"
#include "../../src/figure/triangle/Triangle.hpp"
#include "../../src/util/string_to_figure/StringToFigure.hpp"

#include <iostream>

TEST_CASE("StringToFigure::create_figure creates valid Triangle", "[StringToFigure]")
{
    SECTION("Lowercase triangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase TRIANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("TRIANGLE 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case TrIaNgLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("TrIaNgLe 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3 4 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with extra whitespace")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle  3.0   4.0    5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("create_figure creates valid Circle", "[StringToFigure]")
{
    SECTION("Lowercase circle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase CIRCLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("CIRCLE 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case CiRcLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("CiRcLe 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Circle with integer parameter")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("create_figure creates valid Rectangle", "[StringToFigure]")
{
    SECTION("Lowercase rectangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("rectangle 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase RECTANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("RECTANGLE 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case ReCtAnGlE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("ReCtAnGlE 4.0 6.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Rectangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("rectangle 4 6");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("create_figure throws on invalid parameter count", "[StringToFigure]")
{
    SECTION("Triangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("triangle 3.0 4.0"), "triangle requires three parameters");
    }

    SECTION("Triangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 4.0 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("triangle 3.0 4.0 5.0 6.0"),
                            "triangle requires three parameters");
    }

    SECTION("Circle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("circle"), "circle requires one parameter");
    }

    SECTION("Circle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("circle 5.0 6.0"), "circle requires one parameter");
    }

    SECTION("Rectangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("rectangle 4.0"), "rectangle requires two parameters");
    }

    SECTION("Rectangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0 6.0 8.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("rectangle 4.0 6.0 8.0"),
                            "rectangle requires two parameters");
    }
}

TEST_CASE("create_figure throws on invalid figure name", "[StringToFigure]")
{
    SECTION("Unknown figure name")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("square 5.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::create_figure("square 5.0"), "Invalid figure string");
    }

    SECTION("Empty string")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure(""), std::invalid_argument);
    }

    SECTION("Only whitespace")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("   "), std::invalid_argument);
    }

    SECTION("Random characters")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("asdfghjkl 1 2 3"), std::invalid_argument);
    }
}

TEST_CASE("create_figure throws on invalid numeric parameters", "[StringToFigure]")
{
    SECTION("Non-numeric parameter for triangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("triangle 3.0 abc 5.0"), std::invalid_argument);
    }

    SECTION("Non-numeric parameter for circle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("circle xyz"), std::invalid_argument);
    }

    SECTION("Non-numeric parameter for rectangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::create_figure("rectangle 4.0 notanumber"), std::invalid_argument);
    }
}

TEST_CASE("create_figure handles negative and zero values", "[StringToFigure]")
{
    SECTION("Negative parameter for circle")
    {
        auto result = []() { return StringToFigure::create_figure("circle -5.0"); };
        REQUIRE_THROWS_AS(result(), std::invalid_argument);
    }

    SECTION("Zero parameter for rectangle")
    {
        auto result = []() { return StringToFigure::create_figure("rectangle 0.0 5.0"); };
        REQUIRE_THROWS_AS(result(), std::invalid_argument);
    }
}

TEST_CASE("create_figure handles scientific notation", "[StringToFigure]")
{
    SECTION("Circle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("circle 1.5e2");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::create_figure("triangle 3e0 4e0 5e0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("is_double validates numeric strings", "[StringToFigure]")
{
    SECTION("Valid doubles")
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

    SECTION("Invalid doubles")
    {
        REQUIRE_FALSE(StringToFigure::is_double("abc"));
        REQUIRE_FALSE(StringToFigure::is_double("3.14.15"));
        REQUIRE_FALSE(StringToFigure::is_double(""));
        REQUIRE_FALSE(StringToFigure::is_double("   "));
        REQUIRE_FALSE(StringToFigure::is_double("12abc"));
        REQUIRE_FALSE(StringToFigure::is_double("abc12"));
    }

    SECTION("Edge cases")
    {
        REQUIRE(StringToFigure::is_double("inf"));
        REQUIRE(StringToFigure::is_double("-inf"));
        REQUIRE(StringToFigure::is_double("nan"));
    }
}

TEST_CASE("create_figure handles decimal values", "[StringToFigure]")
{
    SECTION("Triangle with decimal values")
    {
        auto figure = StringToFigure::create_figure("triangle 3.5 4.7 5.9");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Circle with decimal value")
    {
        auto figure = StringToFigure::create_figure("circle 2.71828");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Rectangle with decimal values")
    {
        auto figure = StringToFigure::create_figure("rectangle 1.414 2.718");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}
