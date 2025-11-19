#include <iostream>

#include "../src/application/Application.hpp"

int main()
{
    try
    {
        Application::getInstance().run();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
