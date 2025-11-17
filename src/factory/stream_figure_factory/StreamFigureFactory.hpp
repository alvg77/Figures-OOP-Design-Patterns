#ifndef FIGURES_STREAMFIGUREFACTORY_HPP
#define FIGURES_STREAMFIGUREFACTORY_HPP

#include <istream>
#include <memory>

#include "../FigureFactory.hpp"

class StreamFigureFactory final : public FigureFactory {
private:
    std::unique_ptr<std::istream> is;

public:
    explicit StreamFigureFactory(std::unique_ptr<std::istream> is);

    std::unique_ptr<Figure> create() override;
};

#endif //FIGURES_STREAMFIGUREFACTORY_HPP
