#ifndef FIGURES_APPLICATION_HPP
#define FIGURES_APPLICATION_HPP

#include <string>
#include <vector>

class Application {
    static void split(const std::string &input, std::vector<std::string> &output);

public:
    static void run();
};

#endif //FIGURES_APPLICATION_HPP
