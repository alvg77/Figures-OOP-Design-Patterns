#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <random>
#include <ctime>

#include "../../src/util/figure_util/FigureUtil.hpp"

constexpr int SAMPLE_SIZE = 100;

TEST_CASE("Valid strings convert to correct figure types", "FigureUtil")
{
    SECTION("triangle")
    {
        REQUIRE(FigureUtil::strToFigure("triangle") == FigureUtil::TRIANGLE);
    }

    SECTION("circle")
    {
        REQUIRE(FigureUtil::strToFigure("circle") == FigureUtil::CIRCLE);
    }

    SECTION("rectangle")
    {
        REQUIRE(FigureUtil::strToFigure("rectangle") == FigureUtil::RECTANGLE);
    }
}

TEST_CASE("strToFigure rejects invalid strings", "FigureUtil")
{
    const std::string str = GENERATE("invalid", "", "Triangle", "CIRCLE", "   rectangle", "circle   ", "asy9h237@@f23");

    REQUIRE_THROWS_WITH(FigureUtil::strToFigure(str), "Invalid figure type: '" + str + "'");
}

TEST_CASE("Figure types return correct parameter counts", "FigureUtil")
{
    REQUIRE(FigureUtil::getFigureParams(FigureUtil::TRIANGLE) == 3);
    REQUIRE(FigureUtil::getFigureParams(FigureUtil::CIRCLE) == 1);
    REQUIRE(FigureUtil::getFigureParams(FigureUtil::RECTANGLE) == 2);
}

TEST_CASE("Generates valid figure types", "FigureUtil")
{
    std::mt19937_64 rng(time(nullptr));

    for (int i = 0; i < 100; ++i)
    {
        const FigureUtil::FigureType type = FigureUtil::getRandomFigureType(rng);

        REQUIRE((type == FigureUtil::TRIANGLE || type == FigureUtil::CIRCLE || type == FigureUtil::RECTANGLE));
    }
}

TEST_CASE("Generates all three figure types over multiple calls", "FigureUtil")
{
    std::mt19937_64 rng(time(nullptr));

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        bool hasTriangle = false;
        bool hasCircle = false;
        bool hasRectangle = false;

        for (int j = 0; j < SAMPLE_SIZE; ++j)
        {
            const FigureUtil::FigureType type = FigureUtil::getRandomFigureType(rng);

            if (type == FigureUtil::TRIANGLE)
                hasTriangle = true;
            if (type == FigureUtil::CIRCLE)
                hasCircle = true;
            if (type == FigureUtil::RECTANGLE)
                hasRectangle = true;
        }

        REQUIRE(hasTriangle);
        REQUIRE(hasCircle);
        REQUIRE(hasRectangle);
    }
}

TEST_CASE("strToFigure and getFigureParams work together correctly", "FigureUtil")
{
    const FigureUtil::FigureType triangleType = FigureUtil::strToFigure("triangle");
    const FigureUtil::FigureType circleType = FigureUtil::strToFigure("circle");
    const FigureUtil::FigureType rectangleType = FigureUtil::strToFigure("rectangle");

    REQUIRE(FigureUtil::getFigureParams(triangleType) == 3);
    REQUIRE(FigureUtil::getFigureParams(circleType) == 1);
    REQUIRE(FigureUtil::getFigureParams(rectangleType) == 2);
}

TEST_CASE("Random generation produces the same results with the same seeds", "FigureUtil")
{
    std::mt19937_64 rng1(42);
    std::mt19937_64 rng2(42);

    std::vector<FigureUtil::FigureType> sequence1;
    std::vector<FigureUtil::FigureType> sequence2;

    for (int i = 0; i < 20; ++i)
    {
        sequence1.push_back(FigureUtil::getRandomFigureType(rng1));
        sequence2.push_back(FigureUtil::getRandomFigureType(rng2));
    }

    REQUIRE(sequence1 == sequence2);
}

TEST_CASE("Random generation produces different sequences with different seeds", "FigureUtil")
{
    std::mt19937_64 rng1(111);
    std::mt19937_64 rng2(222);

    std::vector<FigureUtil::FigureType> sequence1;
    std::vector<FigureUtil::FigureType> sequence2;

    for (int i = 0; i < 20; ++i)
    {
        sequence1.push_back(FigureUtil::getRandomFigureType(rng1));
        sequence2.push_back(FigureUtil::getRandomFigureType(rng2));
    }

    REQUIRE(sequence1 != sequence2);
}