#ifndef FIGURES_ABSTRACTFACTORY_HPP
#define FIGURES_ABSTRACTFACTORY_HPP

#include <fstream>
#include <vector>

#include "../FigureFactory.hpp"

class AbstractFactory {
public:
    static std::unique_ptr<FigureFactory> getFactory(std::vector<std::string> &inputType);
};

#endif //FIGURES_ABSTRACTFACTORY_HPP