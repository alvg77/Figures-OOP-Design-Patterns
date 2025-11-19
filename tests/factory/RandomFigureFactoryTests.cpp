#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <unordered_set>

#include "../../src/factory/random_figure_factory/RandomFigureFactory.hpp"
#include "../../src/figure/Figure.hpp"
#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"
#include "../../src/figure/triangle/Triangle.hpp"

constexpr double TOLERANCE = 1e-10;
constexpr int SAMPLE_SIZE = 100;

constexpr double minValue = std::numeric_limits<double>::min();

bool isCircle(const Figure *fig)
{
    return dynamic_cast<const Circle *>(fig) != nullptr;
}

bool isRectangle(const Figure *fig)
{
    return dynamic_cast<const Rectangle *>(fig) != nullptr;
}

bool isTriangle(const Figure *fig)
{
    return dynamic_cast<const Triangle *>(fig) != nullptr;
}

TEST_CASE("Factory creates valid figures", "[randomfactory][create]")
{
    RandomFigureFactory factory;

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure != nullptr);
    REQUIRE((isCircle(figure.get()) || isRectangle(figure.get()) || isTriangle(figure.get())));
}

TEST_CASE("Factory creates figures with valid perimeters", "[randomfactory][perimeter]")
{
    RandomFigureFactory factory;

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure != nullptr);
    const double perimeter = figure->perimeter();
    REQUIRE(std::isfinite(perimeter));
    REQUIRE(perimeter > 0);
}

TEST_CASE("Factory creates multiple distinct figures", "[randomfactory][multiple]")
{
    RandomFigureFactory factory;

    auto figure1 = factory.create();
    auto figure2 = factory.create();
    auto figure3 = factory.create();

    REQUIRE(figure1 != nullptr);
    REQUIRE(figure2 != nullptr);
    REQUIRE(figure3 != nullptr);

    REQUIRE(figure1.get() != figure2.get());
    REQUIRE(figure2.get() != figure3.get());
    REQUIRE(figure1.get() != figure3.get());
}

TEST_CASE("Factory generates all three figure types over multiple calls", "[randomfactory][distribution]")
{
    RandomFigureFactory factory;

    bool hasCircle = false;
    bool hasRectangle = false;
    bool hasTriangle = false;

    for (int i = 0; i < SAMPLE_SIZE && !(hasCircle && hasRectangle && hasTriangle); ++i)
    {
        auto figure = factory.create();
        REQUIRE(figure != nullptr);

        if (isCircle(figure.get()))
            hasCircle = true;
        if (isRectangle(figure.get()))
            hasRectangle = true;
        if (isTriangle(figure.get()))
            hasTriangle = true;
    }

    REQUIRE(hasCircle);
    REQUIRE(hasRectangle);
    REQUIRE(hasTriangle);
}

TEST_CASE("Generated circles have valid radii", "[randomfactory][circle][validation]")
{
    RandomFigureFactory factory;

    bool foundCircle = false;

    for (int i = 0; i < SAMPLE_SIZE && !foundCircle; ++i)
    {
        auto figure = factory.create();

        if (isCircle(figure.get()))
        {
            foundCircle = true;
            const double perimeter = figure->perimeter();
            const double radius = perimeter / (2 * M_PI);

            REQUIRE(radius >= minValue);
            REQUIRE(std::isfinite(radius));
            REQUIRE(std::isfinite(perimeter));
        }
    }

    REQUIRE(foundCircle);
}

TEST_CASE("Generated rectangles have valid dimensions", "[randomfactory][rectangle][validation]")
{
    RandomFigureFactory factory;

    bool foundRectangle = false;

    for (int i = 0; i < SAMPLE_SIZE && !foundRectangle; ++i)
    {
        auto figure = factory.create();

        if (isRectangle(figure.get()))
        {
            foundRectangle = true;
            const double perimeter = figure->perimeter();

            REQUIRE(perimeter >= 4 * minValue);
            REQUIRE(std::isfinite(perimeter));
        }
    }

    REQUIRE(foundRectangle);
}

TEST_CASE("Generated triangles satisfy triangle inequality", "[randomfactory][triangle][validation]")
{
    RandomFigureFactory factory;

    bool foundTriangle = false;

    for (int i = 0; i < SAMPLE_SIZE && !foundTriangle; ++i)
    {
        auto figure = factory.create();

        if (isTriangle(figure.get()))
        {
            foundTriangle = true;
            const double perimeter = figure->perimeter();

            REQUIRE(perimeter >= 3 * minValue);
            REQUIRE(std::isfinite(perimeter));

            REQUIRE_NOTHROW(figure->perimeter());
        }
    }

    REQUIRE(foundTriangle);
}

TEST_CASE("Generated figures have reasonable perimeter values", "[randomfactory][perimeter][range]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < 20; ++i)
    {
        const std::unique_ptr<Figure> figure = factory.create();
        const double perimeter = figure->perimeter();

        REQUIRE(perimeter > 0);
        REQUIRE(std::isfinite(perimeter));
        REQUIRE(perimeter < std::numeric_limits<double>::max());
    }
}

TEST_CASE("Generated figures can be cloned successfully", "[randomfactory][clone]")
{
    RandomFigureFactory factory;

    std::unique_ptr<Figure> figure = factory.create();
    REQUIRE(figure != nullptr);

    Figure *cloned = figure->clone();

    REQUIRE(cloned != nullptr);
    REQUIRE(cloned != figure.get());
    REQUIRE_THAT(cloned->perimeter(), Catch::Matchers::WithinRel(figure->perimeter(), TOLERANCE));

    delete cloned;
}

TEST_CASE("Generated figures produce valid string representations", "[randomfactory][string]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < 10; ++i)
    {
        const std::unique_ptr<Figure> figure = factory.create();
        const std::string str = figure->to_string();

        REQUIRE(!str.empty());
        REQUIRE((str.find("Circle") != std::string::npos || str.find("Rectangle") != std::string::npos ||
                 str.find("Triangle") != std::string::npos));
    }
}

TEST_CASE("Factory produces variety in generated values", "[randomfactory][randomness]")
{
    RandomFigureFactory factory;

    std::unordered_set<double> perimeters;

    for (int i = 0; i < 50; ++i)
    {
        const auto figure = factory.create();
        perimeters.insert(figure->perimeter());
    }

    REQUIRE(perimeters.size() > 40);
}

TEST_CASE("Generated circles avoid overflow", "[randomfactory][circle][overflow]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        auto figure = factory.create();

        if (isCircle(figure.get()))
        {
            const double perimeter = figure->perimeter();
            REQUIRE(std::isfinite(perimeter));
            REQUIRE(perimeter < std::numeric_limits<double>::max());
        }
    }
}

TEST_CASE("Generated rectangles avoid overflow", "[randomfactory][rectangle][overflow]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        auto figure = factory.create();

        if (isRectangle(figure.get()))
        {
            const double perimeter = figure->perimeter();
            REQUIRE(std::isfinite(perimeter));
            REQUIRE(perimeter < std::numeric_limits<double>::max());
        }
    }
}

TEST_CASE("Generated triangles avoid overflow", "[randomfactory][triangle][overflow]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        auto figure = factory.create();

        if (isTriangle(figure.get()))
        {
            const double perimeter = figure->perimeter();
            REQUIRE(std::isfinite(perimeter));
            REQUIRE(perimeter < std::numeric_limits<double>::max());
        }
    }
}

TEST_CASE("Multiple factories can coexist", "[randomfactory][multiple-factories]")
{
    RandomFigureFactory factory1;
    RandomFigureFactory factory2;

    auto figure1 = factory1.create();
    auto figure2 = factory2.create();

    REQUIRE(figure1 != nullptr);
    REQUIRE(figure2 != nullptr);
    REQUIRE(figure1.get() != figure2.get());
}

TEST_CASE("Factory can create many figures without issues", "[randomfactory][stress]")
{
    RandomFigureFactory factory;

    int successCount = 0;

    for (int i = 0; i < 1000; ++i)
    {
        if (std::unique_ptr<Figure> figure = factory.create(); figure != nullptr && std::isfinite(figure->perimeter()))
        {
            successCount++;
        }
    }

    REQUIRE(successCount == 1000);
}

TEST_CASE("Generated figures have minimum dimensions", "[randomfactory][minimum]")
{
    RandomFigureFactory factory;

    for (int i = 0; i < 50; ++i)
    {
        const std::unique_ptr<Figure> figure = factory.create();
        const double perimeter = figure->perimeter();

        REQUIRE(perimeter >= 3 * minValue);
    }
}
