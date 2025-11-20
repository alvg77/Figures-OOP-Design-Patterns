#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include "../../src/factory/FigureFactory.hpp"
#include "../../src/factory/abstract_factory/AbstractFactory.hpp"
#include "../../src/factory/random_figure_factory/RandomFigureFactory.hpp"
#include "../../src/factory/stream_figure_factory/StreamFigureFactory.hpp"

#include <filesystem>

bool isRandomFigureFactory(const FigureFactory *figfactory)
{
    return dynamic_cast<const RandomFigureFactory *>(figfactory);
}

bool isStreamFigureFactory(const FigureFactory *figfactory)
{
    return dynamic_cast<const StreamFigureFactory *>(figfactory);
}

TEST_CASE("Returns nullptr for empty input", "[AbstractFactory]")
{
    std::vector<std::string> input;

    REQUIRE(AbstractFactory::getFactory(input) == nullptr);
}

TEST_CASE("Creates RandomFigureFactory for 'random' input", "[AbstractFactory]")
{
    std::vector<std::string> input = {"random"};

    REQUIRE(isRandomFigureFactory(AbstractFactory::getFactory(input).get()));
}

TEST_CASE("Creates StreamFigureFactory for 'stdin' input", "[AbstractFactory]")
{
    std::vector<std::string> input = {"stdin"};

    REQUIRE(isStreamFigureFactory(AbstractFactory::getFactory(input).get()));
}

TEST_CASE("Creates StreamFigureFactory with file stream for 'file <filename>' input", "[AbstractFactory]")
{
    std::string filename = "test_input.txt";

    std::ofstream testFile(filename);
    testFile << "Circle 5.0\n";
    testFile << "Rectangle 5.0 5.0\n";

    {
        std::vector<std::string> input = {"file", filename};
        const std::unique_ptr<FigureFactory> factory = AbstractFactory::getFactory(input);
        REQUIRE(isStreamFigureFactory(factory.get()));

        testFile.close();
    }

    std::filesystem::remove(filename);
}

TEST_CASE("Throws exception for 'file' without filename or with many filenames", "[AbstractFactory]")
{
    std::vector<std::string> input =
        GENERATE(values<std::vector<std::string>>({{"file"}, {"file", "a", "b", "c"}, {"file", "a", "b"}}));

    CAPTURE(input);

    REQUIRE_THROWS_WITH(AbstractFactory::getFactory(input), "Invalid number of arguments for 'file' choice");
}

TEST_CASE("Throws exception for unrecognized input type", "[AbstractFactory]")
{
    std::vector<std::string> input =
        GENERATE(values<std::vector<std::string>>({{"asfaslhj"}, {"asdf", "asdf", "asdf"}}));

    REQUIRE_THROWS_WITH(AbstractFactory::getFactory(input), "Invalid input");
}