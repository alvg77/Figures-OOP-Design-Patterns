Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.11.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(triangle-tests figure/TriangleTests.cpp
        figure/CircleTests.cpp
        util/StringToFigureTests.cpp
        factory/StreamFactoryTests.cpp
)

target_link_libraries(triangle-tests PRIVATE figures_lib Catch2::Catch2WithMain)