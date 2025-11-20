#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"
#include "../../src/figure/triangle/Triangle.hpp"
#include "../../src/util/string_to_figure/StringToFigure.hpp"

#include <iostream>
#include <oneapi/tbb/info.h>

TEST_CASE("StringToFigure::createFigure creates valid Triangle")
{
    SECTION("Lowercase triangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("triangle 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase TRIANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("TRIANGLE 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case TrIaNgLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("TrIaNgLe 3.0 4.0 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("triangle 3 4 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with extra whitespace")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("triangle  3.0   4.0    5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("createFigure creates valid Circle")
{
    SECTION("Lowercase circle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("circle 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase CIRCLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("CIRCLE 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case CiRcLe")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("CiRcLe 5.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Circle with integer parameter")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("circle 5");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("createFigure creates valid Rectangle")
{
    SECTION("Lowercase rectangle")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("rectangle 1.0 2.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Uppercase RECTANGLE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("RECTANGLE 1.0 2.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Mixed case ReCtAnGlE")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("ReCtAnGlE 1.0 2.0");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }

    SECTION("Rectangle with integer parameters")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("rectangle 1 2");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("createFigure throws on invalid parameter count")
{
    SECTION("Triangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("triangle 3.0 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("triangle 3.0 4.0"), "Triangle requires three parameters");
    }

    SECTION("Triangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("triangle 3.0 4.0 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("triangle 3.0 4.0 5.0 6.0"),
                            "Triangle requires three parameters");
    }

    SECTION("Circle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("circle"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("circle"), "Circle requires one parameter");
    }

    SECTION("Circle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("circle 5.0 6.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("circle 5.0 6.0"), "Circle requires one parameter");
    }

    SECTION("Rectangle with too few parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("rectangle 4.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("rectangle 4.0"), "Rectangle requires two parameters");
    }

    SECTION("Rectangle with too many parameters")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("rectangle 4.0 6.0 8.0"), std::invalid_argument);
        REQUIRE_THROWS_WITH(StringToFigure::createFigure("rectangle 4.0 6.0 8.0"),
                            "Rectangle requires two parameters");
    }
}

TEST_CASE("createFigure throws on invalid figure name")
{
    SECTION("Unknown figure name")
    {
        REQUIRE(StringToFigure::createFigure("square 5.0") == nullptr);
    }

    SECTION("Empty string")
    {
        REQUIRE(StringToFigure::createFigure("") == nullptr);
    }

    SECTION("Only whitespace")
    {
        REQUIRE(StringToFigure::createFigure("   ") == nullptr);
    }

    SECTION("Random characters")
    {
        REQUIRE(StringToFigure::createFigure("asdfghjkl 1 2 3") == nullptr);
    }
}

TEST_CASE("createFigure throws on invalid numeric parameters")
{
    SECTION("Non-numeric parameter for triangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("triangle 3.0 abc 5.0"), std::invalid_argument);
    }

    SECTION("Non-numeric parameter for circle")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("circle xyz"), std::invalid_argument);
    }

    SECTION("Non-numeric parameter for rectangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("rectangle 4.0 notanumber"), std::invalid_argument);
    }
}

TEST_CASE("createFigure handles negative and zero values")
{
    SECTION("Negative parameter for circle")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("circle -5.0"), std::invalid_argument);
    }

    SECTION("Zero parameter for rectangle")
    {
        REQUIRE_THROWS_AS(StringToFigure::createFigure("rectangle 0.0 5.0"), std::invalid_argument);
    }
}

TEST_CASE("createFigure handles scientific notation")
{
    SECTION("Circle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("circle 1.5e2");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Triangle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("triangle 1.5e+10 2.5e+10 3.5e+10");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Rectangle with scientific notation")
    {
        std::unique_ptr<Figure> figure = StringToFigure::createFigure("rectangle 1e+130 2e-131");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}

TEST_CASE("isDouble validates numeric strings")
{
    SECTION("Valid doubles")
    {
        REQUIRE(StringToFigure::isDouble("3.14"));
        REQUIRE(StringToFigure::isDouble("42"));
        REQUIRE(StringToFigure::isDouble("-5.5"));
        REQUIRE(StringToFigure::isDouble("0.0"));
        REQUIRE(StringToFigure::isDouble("1e10"));
        REQUIRE(StringToFigure::isDouble("1.5e-131"));
        REQUIRE(StringToFigure::isDouble(".5"));
        REQUIRE(StringToFigure::isDouble("5."));
    }

    SECTION("Invalid doubles")
    {
        REQUIRE_FALSE(StringToFigure::isDouble("abc"));
        REQUIRE_FALSE(StringToFigure::isDouble("3.14.15"));
        REQUIRE_FALSE(StringToFigure::isDouble(""));
        REQUIRE_FALSE(StringToFigure::isDouble("   "));
        REQUIRE_FALSE(StringToFigure::isDouble("12abc"));
        REQUIRE_FALSE(StringToFigure::isDouble("abc12"));
    }

    SECTION("Edge cases")
    {
        REQUIRE(StringToFigure::isDouble("inf"));
        REQUIRE(StringToFigure::isDouble("-inf"));
        REQUIRE(StringToFigure::isDouble("nan"));
    }
}

TEST_CASE("createFigure handles decimal values")
{
    SECTION("Triangle with decimal values")
    {
        auto figure = StringToFigure::createFigure("triangle 3.5 4.7 5.9");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Triangle *>(figure.get()) != nullptr);
    }

    SECTION("Circle with decimal value")
    {
        auto figure = StringToFigure::createFigure("circle 2.71828");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Circle *>(figure.get()) != nullptr);
    }

    SECTION("Rectangle with decimal values")
    {
        auto figure = StringToFigure::createFigure("rectangle 1.414 2.718");
        REQUIRE(figure != nullptr);
        REQUIRE(dynamic_cast<Rectangle *>(figure.get()) != nullptr);
    }
}
