#ifndef FIGURES_STRINGTOFIGURE_HPP
#define FIGURES_STRINGTOFIGURE_HPP

#include <string>
#include <memory>

#include "../../figure/Figure.hpp"

class StringToFigure
{
  public:
    static std::unique_ptr<Figure> createFigure(const std::string &representation);
};

#endif // FIGURES_STRINGTOFIGURE_HPP
