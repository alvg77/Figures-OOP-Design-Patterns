include(FetchContent)

fetchContent_declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)

fetchContent_makeAvailable(Catch2)

set(FIGURES_TEST_SOURCES
        figure/TriangleTests.cpp
        figure/RectangleTests.cpp
        figure/CircleTests.cpp
        util/StringToFigureTests.cpp
        util/FigureUtilTests.cpp
        util/StringConvertibleTests.cpp
        factory/RandomFigureFactoryTests.cpp
        factory/StreamFigureFactoryTests.cpp
        factory/AbstractFactoryTests.cpp
)

add_executable(figures-tests ${FIGURES_TEST_SOURCES})

target_link_libraries(figures-tests PRIVATE
        figures_figure
        figures_util
        figures_factory
        Catch2::Catch2WithMain
)

