#ifndef FIGURES_RANDOMFIGUREFACTORY_HPP
#define FIGURES_RANDOMFIGUREFACTORY_HPP

#include <random>

#include "../FigureFactory.hpp"
#include "../../figure/triangle/Triangle.hpp"
#include "../../figure/circle/Circle.hpp"
#include "../../figure/rectangle/Rectangle.hpp"

class RandomFigureFactory final : public FigureFactory{
private:
    static const unsigned seed;
    std::mt19937_64 rng;

    std::unique_ptr<Triangle> generateTriangle();
    std::unique_ptr<Circle> generateCircle();
    std::unique_ptr<Rectangle> generateRectangle();

public:
    RandomFigureFactory();

    std::unique_ptr<Figure> create() override;
};

#endif //FIGURES_RANDO IGUREFACTORY_HPP
