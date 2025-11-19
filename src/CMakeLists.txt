set(FIGURES_SOURCES figure/Figure.hpp
        util/Clonable.hpp
        util/StringConvertible.hpp
        figure/triangle/Triangle.cpp
        figure/triangle/Triangle.hpp
        figure/rectangle/Rectangle.cpp
        figure/rectangle/Rectangle.hpp
        figure/circle/Circle.cpp
        figure/circle/Circle.hpp
        factory/FigureFactory.hpp
        factory/abstract_factory/AbstractFactory.cpp
        factory/abstract_factory/AbstractFactory.hpp
        factory/random_figure_factory/RandomFigureFactory.cpp
        factory/random_figure_factory/RandomFigureFactory.hpp
        factory/stream_figure_factory/StreamFigureFactory.cpp
        factory/stream_figure_factory/StreamFigureFactory.hpp
        util/string_to_figure/StringToFigure.cpp
        util/string_to_figure/StringToFigure.hpp
        util/figure_util/FigureUtil.cpp
        util/figure_util/FigureUtil.hpp
        application/Application.cpp
        application/Application.hpp
        figure/Figure.cpp)

add_library(figures_lib ${FIGURES_SOURCES})

add_executable(figures main.cpp)
target_link_libraries(figures figures_lib)
