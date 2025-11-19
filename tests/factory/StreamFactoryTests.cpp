#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cmath>
#include <memory>
#include <sstream>

#include "../../src/factory/stream_figure_factory/StreamFigureFactory.hpp"

constexpr double TOLERANCE = 1e-10;

struct ValidFigureData
{
    std::string input;
    std::string expectedType;
    double expectedPerimeter;
};

TEST_CASE("Valid figures are created correctly from stream", "[streamfactory][create]")
{
    auto [input, expectedType, expectedPerimeter] =
        GENERATE(values<ValidFigureData>({{"circle 5", "Circle", 10.0 * M_PI},
                                          {"Circle 10", "Circle", 20.0 * M_PI},
                                          {"CIRCLE 7.5", "Circle", 15.0 * M_PI},
                                          {"rectangle 10 20", "Rectangle", 60.0},
                                          {"Rectangle 5 10", "Rectangle", 30.0},
                                          {"RECTANGLE 12 12", "Rectangle", 48.0},
                                          {"triangle 3 4 5", "Triangle", 12.0},
                                          {"Triangle 5 5 5", "Triangle", 15.0},
                                          {"TRIANGLE 10 10 10", "Triangle", 30.0}}));

    CAPTURE(input, expectedType, expectedPerimeter);

    auto stream = std::make_unique<std::istringstream>(input);
    StreamFigureFactory factory(std::move(stream));

    auto figure = factory.create();

    REQUIRE(figure != nullptr);
    REQUIRE(figure->to_string().find(expectedType) != std::string::npos);
    REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(expectedPerimeter, TOLERANCE));
}

TEST_CASE("Factory handles case-insensitive figure names", "[streamfactory][case]")
{
    SECTION("Lowercase circle")
    {
        auto stream = std::make_unique<std::istringstream>("circle 5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Uppercase circle")
    {
        auto stream = std::make_unique<std::istringstream>("CIRCLE 5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Mixed case circle")
    {
        auto stream = std::make_unique<std::istringstream>("CiRcLe 5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }
}

TEST_CASE("Factory handles decimal parameters", "[streamfactory][decimal]")
{
    SECTION("Circle with decimal radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle 7.5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(15.0 * M_PI, TOLERANCE));
    }

    SECTION("Rectangle with decimal sides")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle 5.5 7.3");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
    }

    SECTION("Triangle with decimal sides")
    {
        auto stream = std::make_unique<std::istringstream>("triangle 3.5 4.5 5.5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
    }
}

TEST_CASE("Factory returns nullptr on empty stream", "[streamfactory][empty]")
{
    auto stream = std::make_unique<std::istringstream>("");
    StreamFigureFactory factory(std::move(stream));

    auto figure = factory.create();

    REQUIRE(figure == nullptr);
}

TEST_CASE("Factory returns nullptr on whitespace-only stream", "[streamfactory][empty]")
{
    auto stream = std::make_unique<std::istringstream>("   \n\t  ");
    StreamFigureFactory factory(std::move(stream));

    auto figure = factory.create();

    REQUIRE(figure == nullptr);
}

TEST_CASE("Factory handles multiple figures from same stream", "[streamfactory][multiple]")
{
    auto stream = std::make_unique<std::istringstream>("circle 5\nrectangle 10 20\ntriangle 3 4 5");
    StreamFigureFactory factory(std::move(stream));

    auto figure1 = factory.create();
    REQUIRE(figure1 != nullptr);
    REQUIRE_THAT(figure1->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));

    auto figure2 = factory.create();
    REQUIRE(figure2 != nullptr);
    REQUIRE_THAT(figure2->perimeter(), Catch::Matchers::WithinRel(60.0, TOLERANCE));

    auto figure3 = factory.create();
    REQUIRE(figure3 != nullptr);
    REQUIRE_THAT(figure3->perimeter(), Catch::Matchers::WithinRel(12.0, TOLERANCE));

    auto figure4 = factory.create();
    REQUIRE(figure4 == nullptr);
}

TEST_CASE("Factory can be constructed with nullptr stream", "[streamfactory][nullptr]")
{
    REQUIRE_NOTHROW(StreamFigureFactory(nullptr));
}

TEST_CASE("Factory handles extra whitespace in input", "[streamfactory][whitespace]")
{
    SECTION("Extra spaces between parameters")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle    10     20");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(60.0, TOLERANCE));
    }

    SECTION("Leading whitespace")
    {
        auto stream = std::make_unique<std::istringstream>("   circle 5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Tab separators")
    {
        auto stream = std::make_unique<std::istringstream>("triangle\t3\t4\t5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(12.0, TOLERANCE));
    }
}

TEST_CASE("Factory throws on invalid figure type", "[streamfactory][invalid]")
{
    auto stream = std::make_unique<std::istringstream>("pentagon 5");
    StreamFigureFactory factory(std::move(stream));

    REQUIRE_THROWS(factory.create());
}

TEST_CASE("Factory throws on insufficient parameters", "[streamfactory][invalid]")
{
    SECTION("Circle without radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with one parameter")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle 10");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Triangle with two parameters")
    {
        auto stream = std::make_unique<std::istringstream>("triangle 3 4");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }
}

TEST_CASE("Factory throws on invalid parameter values", "[streamfactory][invalid]")
{
    SECTION("Circle with negative radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle -5");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Circle with zero radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle 0");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with negative dimensions")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle -10 20");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with zero dimensions")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle 0 20");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Triangle violating triangle inequality")
    {
        auto stream = std::make_unique<std::istringstream>("triangle 1 2 10");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }
}

TEST_CASE("Factory throws on non-numeric parameters", "[streamfactory][invalid]")
{
    SECTION("Circle with text radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle abc");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with mixed valid and invalid parameters")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle 10 xyz");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Triangle with special characters")
    {
        auto stream = std::make_unique<std::istringstream>("triangle 3 4 @#$");
        StreamFigureFactory factory(std::move(stream));

        REQUIRE_THROWS(factory.create());
    }
}

TEST_CASE("Factory handles scientific notation", "[streamfactory][scientific]")
{
    SECTION("Circle with scientific notation radius")
    {
        auto stream = std::make_unique<std::istringstream>("circle 1e-5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(2e-5 * M_PI, TOLERANCE));
    }

    SECTION("Rectangle with large scientific notation values")
    {
        auto stream = std::make_unique<std::istringstream>("rectangle 1e5 2e5");
        StreamFigureFactory factory(std::move(stream));
        auto figure = factory.create();

        REQUIRE(figure != nullptr);
    }
}