#ifndef FIGURES_FIGURE_UTIL_HPP
#define FIGURES_FIGURE_UTIL_HPP

#include <string>
#include <random>

class FigureUtil {
private:
    static constexpr unsigned FIGURE_NUM = 3;

public:
    enum FigureType {
        TRIANGLE = 0,
        CIRCLE,
        RECTANGLE
    };

    static FigureType strToFigure(const std::string &str);

    static unsigned getFigureParams(FigureType type);

    static FigureType getRandomFigureType(std::mt19937_64& rng);
};


#endif //FIGURES_FIGURE_UTIL_HPP
