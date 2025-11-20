#include "Application.hpp"

#include <iostream>
#include <limits>
#include <sstream>

#include "../factory/FigureFactory.hpp"
#include "../factory/abstract_factory/AbstractFactory.hpp"

void Application::split(const std::string &input, std::vector<std::string> &output)
{
    std::stringstream sstream(input);

    std::string value;
    while (sstream >> value)
    {
        output.push_back(value);
    }
}

Application &Application::getInstance()
{
    static Application instance;
    return instance;
}

void Application::run()
{
    loadFigures();
    menu();
}

void Application::loadFigures()
{
    std::cout << "Select input method:\n";
    std::cout << "\t<random>          - generates random figures\n";
    std::cout << "\t<stdin>           - enter figures from stdin\n";
    std::cout << "\t<file 'filename'> - reads figures from file with name 'filename'\n";

    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> splitInputs;
    split(input, splitInputs);

    if (splitInputs.empty())
    {
        throw std::invalid_argument("No input method entered");
    }

    const std::unique_ptr<FigureFactory> factory = AbstractFactory::getFactory(splitInputs);

    if (factory == nullptr)
    {
        throw std::runtime_error("Invalid input method");
    }

    int n;
    do
    {
        std::cout << "Select number of figures - n (if you enter more figures than n, only the first n of them will be processed): ";
        if (!(std::cin >> n))
        {
            std::cout << "Invalid input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            n = 0;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (n <= 0)
        {
            std::cout << "Number of figures should be greater than 0\n";
        }
    } while (n <= 0);

    for (int i = 0; i < n; i++)
    {
        std::unique_ptr<Figure> figure = factory->create();
        if (figure == nullptr)
        {
            throw std::runtime_error("Cannot create figure #" + std::to_string(i));
        }
        figures.push_back(std::move(figure));
    }

    if (splitInputs[0] == "stdin")
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "\n---Figures created---\n";
}

void Application::menu()
{
    int input;
    bool quit = false;
    while (!quit)
    {
        std::cout << "\n1. Display all figures\n";
        std::cout << "2. Clone a figure\n";
        std::cout << "3. Save figures to file\n";
        std::cout << "4. Delete figure\n";
        std::cout << "5. Quit\n";

        if (!(std::cin >> input))
        {
            std::cout << "\nInvalid input. Please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (input)
        {
        case 1:
            displayFigures();
            break;
        case 2:
            cloneFigure();
            break;
        case 3:
            saveToFile();
            break;
        case 4:
            deleteFigure();
            break;
        case 5:
            quit = true;
            break;
        default:
            std::cout << "\nInvalid input. Please try again.\n";
        }
    }
}

void Application::displayFigures() const
{
    std::cout << "------------------------\n";
    for (unsigned i = 0; i < figures.size(); i++)
    {
        std::cout << i << ". " << *figures[i];
    }
    std::cout << "------------------------\n";
}

void Application::cloneFigure()
{
    std::cout << "Please provide the number, corresponding to the figure to be cloned: " << std::endl;

    int input;
    if (!(std::cin >> input))
    {
        std::cout << "Invalid input. Please try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (input >= figures.size() || input < 0)
    {
        std::cout << "Invalid input. No figure corresponds to that number.\n";
        return;
    }

    figures.push_back(std::unique_ptr<Figure>(figures.at(input)->clone()));
    std::cout << "---Figure successfully cloned and added to the end of the list!---\n";
}

void Application::deleteFigure()
{
    std::cout << "Please provide the number, corresponding to the figure to be deleted: " << std::endl;

    int input;
    if (!(std::cin >> input))
    {
        std::cout << "Invalid input. Please try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (input >= figures.size() || input < 0)
    {
        std::cout << "Invalid input. No figure corresponds to that number.\n";
        return;
    }

    figures.erase(figures.begin() + input);
    std::cout << "---Figure successfully deleted!---\n";
}

void Application::saveToFile() const
{
    std::string input;

    std::cout << "Enter output filename (leave blank to cancel): ";
    std::getline(std::cin, input);

    if (!input.empty())
    {
        std::ofstream outputFile(input);

        if (!outputFile.is_open())
        {
            std::cout << "Error opening file. Please try again.\n";
            return;
        }

        for (const std::unique_ptr<Figure> &figure : figures)
        {
            outputFile << *figure;
        }

        if (outputFile.fail())
        {
            std::cout << "Warning: An error occurred while writing figures to file\n";
        } else
        {
            std::cout << "---Figures successfully written!---\n";
        }
    }
}