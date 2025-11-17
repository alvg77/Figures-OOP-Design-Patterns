#ifndef FIGURES_FIGUREFACTORY_HPP
#define FIGURES_FIGUREFACTORY_HPP

#include "../figure/Figure.hpp"

class FigureFactory {
public:
    virtual std::unique_ptr<Figure> create() = 0;
    virtual ~FigureFactory() = default;
};

#endif //FIGURES_FIGUREFACTORY_HPP