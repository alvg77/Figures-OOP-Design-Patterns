#include "AbstractFactory.hpp"

#include "../random_figure_factory/RandomFigureFactory.hpp"
#include "../stream_figure_factory/StreamFigureFactory.hpp"

std::unique_ptr<FigureFactory> AbstractFactory::getFactory(std::vector<std::string> &inputType)
{
    if (inputType.empty())
    {
        return nullptr;
    }

    if (inputType.at(0) == "random")
    {
        return std::make_unique<RandomFigureFactory>();
    }

    if (inputType.at(0) == "stdin")
    {

        return std::make_unique<StreamFigureFactory>(std::unique_ptr<std::istream>(nullptr));
    }

    if (inputType.at(0) == "file")
    {
        if (inputType.size() != 2)
        {
            throw std::invalid_argument("Invalid number of arguments for 'file' choice");
        }

        std::unique_ptr<std::ifstream> file = std::make_unique<std::ifstream>(inputType.at(1));

        if (!file->is_open())
        {
            throw std::runtime_error("Cannot open file: '" + inputType.at(1) + "'");
        }

        return std::make_unique<StreamFigureFactory>(std::move(file));
    }

    throw std::invalid_argument("Invalid input");
}
