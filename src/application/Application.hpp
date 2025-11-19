#ifndef FIGURES_APPLICATION_HPP
#define FIGURES_APPLICATION_HPP

#include "../figure/Figure.hpp"

#include <string>
#include <vector>

class Application
{
    static void split(const std::string &input, std::vector<std::string> &output);
    static Application application;

    std::vector<std::unique_ptr<Figure>> figures;

    Application() = default;

    void loadFigures();
    void menu();
    void displayFigures() const;
    void cloneFigure();
    void deleteFigure();
    void saveToFile() const;

  public:
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    void run();
    static Application &getInstance();
};

#endif // FIGURES_APPLICATION_HPP
