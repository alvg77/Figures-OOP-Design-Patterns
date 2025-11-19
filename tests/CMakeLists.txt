Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.11.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(triangle-tests figure/TriangleTests.cpp)
add_executable(rectangle-tests figure/RectangleTests.cpp)
add_executable(circle-tests figure/CircleTests.cpp)
add_executable(string-to-figure-tests util/StringToFigureTests.cpp)
add_executable(random-factory-tests factory/RandomFigureFactoryTests.cpp)
add_executable(stream-factory-tests factory/StreamFactoryTests.cpp)

target_link_libraries(triangle-tests PRIVATE figures_lib Catch2::Catch2WithMain)
target_link_libraries(rectangle-tests PRIVATE figures_lib Catch2::Catch2WithMain)
target_link_libraries(circle-tests PRIVATE figures_lib Catch2::Catch2WithMain)
target_link_libraries(string-to-figure-tests PRIVATE figures_lib Catch2::Catch2WithMain)
target_link_libraries(random-factory-tests PRIVATE figures_lib Catch2::Catch2WithMain)
target_link_libraries(stream-factory-tests PRIVATE figures_lib Catch2::Catch2WithMain)
