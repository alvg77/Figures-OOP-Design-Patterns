#include "RandomFigureFactory.hpp"

#include <ctime>

#include "../../util/figure_util/FigureUtil.hpp"

const unsigned RandomFigureFactory::seed = std::time(nullptr);

std::unique_ptr<Triangle> RandomFigureFactory::generateTriangle() {
    constexpr double maxValue = std::numeric_limits<double>::max() / 3;
    std::uniform_real_distribution triangleDist(.1, maxValue);

    const double a = triangleDist(rng);
    const double b = triangleDist(rng);

    std::uniform_real_distribution thirdSideDist(std::abs(a - b) + .1, std::min(maxValue, a + b - .1));
    const double c = thirdSideDist(rng);

    return std::make_unique<Triangle>(a, b, c);
}

std::unique_ptr<Circle> RandomFigureFactory::generateCircle() {
    constexpr double maxValue = std::numeric_limits<double>::max() / (M_PI * 2);
    std::uniform_real_distribution<double> circDist (0.1, maxValue);

    return std::make_unique<Circle>(circDist(rng));
}

std::unique_ptr<Rectangle> RandomFigureFactory::generateRectangle() {
    constexpr double maxValue = std::numeric_limits<double>::max() / 4;
    std::uniform_real_distribution<double> rectDist (0.1, maxValue);

    return  std::make_unique<Rectangle>(rectDist(rng), rectDist(rng));
}

RandomFigureFactory::RandomFigureFactory()
    : rng(RandomFigureFactory::seed) {}

std::unique_ptr<Figure> RandomFigureFactory::create() {
    switch (FigureUtil::getRandomFigureType(rng)) {
        case FigureUtil::TRIANGLE:
            return generateTriangle();
        case FigureUtil::CIRCLE:
            return generateCircle();
        case FigureUtil::RECTANGLE:
            return generateRectangle();
        default:
            return nullptr;
    }
}

