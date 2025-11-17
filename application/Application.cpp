#include "Application.hpp"

#include <iostream>
#include <limits>
#include <sstream>

#include "../factory/FigureFactory.hpp"
#include "../factory/abstract_factory/AbstractFactory.hpp"

#include <list>

void Application::split(const std::string &input, std::vector<std::string> &output) {
    std::stringstream sstream(input);

    std::string value;
    while (sstream >> value) {
        output.push_back(value);
    }
}

void Application::run() {
    std::cout << "Select input method:\n";
    std::cout << "\t<random>          - generates random figures\n";
    std::cout << "\t<stdin>           - enter figures from stdin\n";
    std::cout << "\t<file 'filename'> - reads figures from file with name 'filename'\n";

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> splitInputs;
    split(input, splitInputs);

    const std::unique_ptr<FigureFactory> factory = AbstractFactory::getFactory(splitInputs);

    int n;
    do {
        std::cout << "\nSelect number of figures: ";
        std::cin >> n;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (n <= 0) {
            std::cout << "\nNumber of figures should be greater than 0\n";
        }
    } while (n <= 0);

    std::vector<std::unique_ptr<Figure>> figures;
    for (int i = 0; i < n; i++) {
        std::unique_ptr<Figure> figure = factory->create();
        if (figure == nullptr) {
            throw std::runtime_error("Cannot create figure #" + std::to_string(i));
        }
        figures.push_back(std::move(figure));
    }
    std::cout << "\n---Figures created---\n";

    std::cout << "\nEnter output filename or leave blank to print to console: ";
    std::getline(std::cin, input);

    if (!input.empty()) {
        std::ofstream outputFile(input);

        for (const std::unique_ptr<Figure> &figure : figures) {
            outputFile << *figure;
        }
    } else {
        for (const std::unique_ptr<Figure> &figure : figures) {
            std::cout << *figure;
        }
    }
}
