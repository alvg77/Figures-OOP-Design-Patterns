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

TEST_CASE("Valid figures are created correctly from stream")
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

    StreamFigureFactory factory(std::make_unique<std::istringstream>(input));

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure != nullptr);
    REQUIRE(figure->toString().find(expectedType) != std::string::npos);
    REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(expectedPerimeter, TOLERANCE));
}

TEST_CASE("Factory handles case-insensitive figure names")
{
    SECTION("Lowercase circle")
    {
        StreamFigureFactory factory(std::make_unique<std::istringstream>("circle 5"));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Uppercase circle")
    {
        StreamFigureFactory factory(std::make_unique<std::istringstream>("CIRCLE 5"));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Mixed case circle")
    {
        StreamFigureFactory factory(std::make_unique<std::istringstream>("CiRcLe 5"));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }
}

TEST_CASE("Factory handles decimal parameters")
{
    SECTION("Circle with decimal radius")
    {
        StreamFigureFactory factory(std::make_unique<std::istringstream>("circle 7.5"));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(15.0 * M_PI, TOLERANCE));
    }

    SECTION("Rectangle with decimal sides")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle 5.5 7.3")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
    }

    SECTION("Triangle with decimal sides")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("triangle 3.5 4.5 5.5")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
    }
}

TEST_CASE("Factory returns nullptr on empty stream")
{
    StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("")));

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure == nullptr);
}

TEST_CASE("Factory returns nullptr on whitespace-only stream")
{
    StreamFigureFactory factory(std::make_unique<std::istringstream>("   \n\t  "));

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure == nullptr);
}

TEST_CASE("Factory handles multiple figures from same stream")
{
    std::unique_ptr<std::istringstream> stream =
        std::make_unique<std::istringstream>("circle 5\nrectangle 10 20\ntriangle 3 4 5");
    StreamFigureFactory factory(std::move(stream));

    std::unique_ptr<Figure> figure1 = factory.create();
    REQUIRE(figure1 != nullptr);
    REQUIRE_THAT(figure1->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));

    std::unique_ptr<Figure> figure2 = factory.create();
    REQUIRE(figure2 != nullptr);
    REQUIRE_THAT(figure2->perimeter(), Catch::Matchers::WithinRel(60.0, TOLERANCE));

    std::unique_ptr<Figure> figure3 = factory.create();
    REQUIRE(figure3 != nullptr);
    REQUIRE_THAT(figure3->perimeter(), Catch::Matchers::WithinRel(12.0, TOLERANCE));

    std::unique_ptr<Figure> figure4 = factory.create();
    REQUIRE(figure4 == nullptr);
}

TEST_CASE("Factory can be constructed with nullptr stream")
{
    REQUIRE_NOTHROW(StreamFigureFactory(nullptr));
}

TEST_CASE("Factory handles extra whitespace in input")
{
    SECTION("Extra spaces between parameters")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle    10     20")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(60.0, TOLERANCE));
    }

    SECTION("Leading whitespace")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("   circle 5")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(10.0 * M_PI, TOLERANCE));
    }

    SECTION("Tab separators")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("triangle\t3\t4\t5")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(12.0, TOLERANCE));
    }
}

TEST_CASE("Factory throws on invalid figure type")
{
    StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("pentagon 5")));

    REQUIRE_THROWS(factory.create());
}

TEST_CASE("Factory throws on insufficient parameters")
{
    SECTION("Circle without radius")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("circle")));

        REQUIRE_THROWS_WITH(factory.create(), "Cannot read from input stream!");
    }

    SECTION("Rectangle with one parameter")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle 10")));

        REQUIRE_THROWS_WITH(factory.create(), "Cannot read from input stream!");
    }

    SECTION("Triangle with two parameters")
    {
        StreamFigureFactory factory(std::make_unique<std::istringstream>("triangle 3 4"));

        REQUIRE_THROWS_WITH(factory.create(), "Cannot read from input stream!");
    }
}

TEST_CASE("Factory throws on invalid parameter values")
{
    SECTION("Circle with negative radius")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("circle -5")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Circle with zero radius")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("circle 0")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with negative dimensions")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle -10 20")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with zero dimensions")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle 0 20")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Triangle violating triangle inequality")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("triangle 1 2 10")));

        REQUIRE_THROWS(factory.create());
    }
}

TEST_CASE("Factory throws on non-numeric parameters")
{
    SECTION("Circle with text radius")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("circle abc")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Rectangle with mixed valid and invalid parameters")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle 10 xyz")));

        REQUIRE_THROWS(factory.create());
    }

    SECTION("Triangle with special characters")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("triangle 3 4 @#$")));

        REQUIRE_THROWS(factory.create());
    }
}

TEST_CASE("Factory handles scientific notation")
{
    SECTION("Circle with scientific notation radius")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("circle 1e-5")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
        REQUIRE_THAT(figure->perimeter(), Catch::Matchers::WithinRel(2e-5 * M_PI, TOLERANCE));
    }

    SECTION("Rectangle with large scientific notation values")
    {
        StreamFigureFactory factory(std::move(std::make_unique<std::istringstream>("rectangle 1e5 2e5")));
        std::unique_ptr<Figure> figure = factory.create();

        REQUIRE(figure != nullptr);
    }
}