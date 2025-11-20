#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <vector>

#include "../../src/factory/random_figure_factory/RandomFigureFactory.hpp"
#include "../../src/figure/Figure.hpp"
#include "../../src/figure/circle/Circle.hpp"
#include "../../src/figure/rectangle/Rectangle.hpp"
#include "../../src/figure/triangle/Triangle.hpp"

constexpr double TOLERANCE = 1e-10;
constexpr int SAMPLE_SIZE = 100;
constexpr int LARGE_SAMPLE = 1000;

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

TEST_CASE("Factory creates valid figures")
{
    RandomFigureFactory factory;

    std::unique_ptr<Figure> figure = factory.create();

    REQUIRE(figure != nullptr);
    REQUIRE((isCircle(figure.get()) || isRectangle(figure.get()) || isTriangle(figure.get())));
}

TEST_CASE("Factory creates multiple distinct figures")
{
    RandomFigureFactory factory;

    std::unique_ptr<Figure> figure1 = factory.create();
    std::unique_ptr<Figure> figure2 = factory.create();
    std::unique_ptr<Figure> figure3 = factory.create();

    REQUIRE(figure1 != nullptr);
    REQUIRE(figure2 != nullptr);
    REQUIRE(figure3 != nullptr);

    REQUIRE(figure1.get() != figure2.get());
    REQUIRE(figure2.get() != figure3.get());
    REQUIRE(figure1.get() != figure3.get());
}

TEST_CASE("Factory generates all three figure types over multiple calls")
{
    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        RandomFigureFactory factory;

        bool hasCircle = false;
        bool hasRectangle = false;
        bool hasTriangle = false;

        for (int j = 0; j < SAMPLE_SIZE && !(hasCircle && hasRectangle && hasTriangle); ++j)
        {
            std::unique_ptr<Figure> figure = factory.create();
            REQUIRE(figure != nullptr);

            if (isCircle(figure.get()))
            {
                hasCircle = true;
            }

            if (isRectangle(figure.get()))
            {
                hasRectangle = true;
            }

            if (isTriangle(figure.get()))
            {
                hasTriangle = true;
            }
        }

        REQUIRE(hasCircle);
        REQUIRE(hasRectangle);
        REQUIRE(hasTriangle);
    }
}

TEST_CASE("Factory generates figures with approximately uniform distribution")
{
    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        RandomFigureFactory factory;

        int circleCount = 0;
        int rectangleCount = 0;
        int triangleCount = 0;

        for (int j = 0; j < LARGE_SAMPLE; ++j)
        {
            std::unique_ptr<Figure> figure = factory.create();
            REQUIRE(figure != nullptr);

            if (isCircle(figure.get()))
            {
                circleCount++;
            }
            else if (isRectangle(figure.get()))
            {
                rectangleCount++;
            }
            else if (isTriangle(figure.get()))
            {
                triangleCount++;
            }
        }

        constexpr double expectedCount = LARGE_SAMPLE / 3.0;
        constexpr double tolerance = expectedCount * 0.2;

        REQUIRE(std::abs(circleCount - expectedCount) < tolerance);
        REQUIRE(std::abs(rectangleCount - expectedCount) < tolerance);
        REQUIRE(std::abs(triangleCount - expectedCount) < tolerance);

        REQUIRE(circleCount + rectangleCount + triangleCount == LARGE_SAMPLE);
    }
}

TEST_CASE("Circles are successfully generated with valid values")
{
    RandomFigureFactory factory;
    int circlesFound = 0;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        std::unique_ptr<Figure> figure;
        REQUIRE_NOTHROW(figure = factory.create());

        if (isCircle(figure.get()))
        {
            circlesFound++;
        }
    }

    REQUIRE(circlesFound >= 20);
}

TEST_CASE("Rectangles are successfully generated with valid values")
{
    RandomFigureFactory factory;
    int rectanglesFound = 0;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        std::unique_ptr<Figure> figure;
        REQUIRE_NOTHROW(figure = factory.create());

        if (isRectangle(figure.get()))
        {
            rectanglesFound++;
        }
    }

    REQUIRE(rectanglesFound >= 20);
}

TEST_CASE("Triangles are successfully generated with valid values")
{
    RandomFigureFactory factory;

    int trianglesFound = 0;

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        std::unique_ptr<Figure> figure;
        REQUIRE_NOTHROW(figure = factory.create());

        if (isTriangle(figure.get()))
        {
            trianglesFound++;
        }
    }

    REQUIRE(trianglesFound >= 20);
}

TEST_CASE("Generated figures have valid perimeter values")
{
    RandomFigureFactory factory;

    for (int i = 0; i < 20; ++i)
    {
        const std::unique_ptr<Figure> figure = factory.create();
        const double perimeter = figure->perimeter();

        REQUIRE(perimeter > 0);
        REQUIRE(perimeter <= std::numeric_limits<double>::max());
    }
}

TEST_CASE("Figures with variety of values are generated")
{
    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        RandomFigureFactory factory;
        std::vector<double> perimeters;

        for (int j = 0; j < LARGE_SAMPLE; ++j)
        {
            const std::unique_ptr<Figure> figure = factory.create();
            perimeters.push_back(figure->perimeter());
        }

        unsigned variety = 0;
        const double initialPerimeter = perimeters.front();
        for (const double p : perimeters)
        {
            if (std::abs(p - initialPerimeter) > TOLERANCE)
            {
                variety++;
            }
        }

        REQUIRE(variety >= LARGE_SAMPLE / 2);
    }
}

TEST_CASE("Multiple factories can coexist and generate separate figures")
{
    RandomFigureFactory factory1;
    RandomFigureFactory factory2;

    std::unique_ptr<Figure> figure1 = factory1.create();
    std::unique_ptr<Figure> figure2 = factory2.create();

    REQUIRE(figure1 != nullptr);
    REQUIRE(figure2 != nullptr);
    REQUIRE(figure1.get() != figure2.get());
}

TEST_CASE("Factory can create many figures")
{
    RandomFigureFactory factory;

    int successCount = 0;

    for (int i = 0; i < LARGE_SAMPLE; ++i)
    {
        std::unique_ptr<Figure> figure;
        REQUIRE_NOTHROW(figure = factory.create());

        if (figure != nullptr)
        {
            successCount++;
        }
    }

    REQUIRE(successCount == LARGE_SAMPLE);
}