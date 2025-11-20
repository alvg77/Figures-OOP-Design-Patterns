Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(Catch2)

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
        figures_lib
        Catch2::Catch2WithMain
)

