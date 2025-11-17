cmake_minimum_required(VERSION 4.0)
project(Figures)

set(CMAKE_CXX_STANDARD 20)

add_executable(Figures src/main.cpp
        src/figure/figure.hpp
        src/util/clonable.hpp
        src/util/StringConvertible.hpp
        src/figure/triangle/Triangle.cpp
        src/figure/triangle/Triangle.hpp
        src/figure/rectangle/Rectangle.cpp
        src/figure/rectangle/Rectangle.hpp
        src/figure/circle/Circle.cpp
        src/figure/circle/Circle.hpp
        src/factory/FigureFactory.hpp
        src/factory/abstract_factory/AbstractFactory.cpp
        src/factory/abstract_factory/AbstractFactory.hpp
        src/factory/random_figure_factory/RandomFigureFactory.cpp
        src/factory/random_figure_factory/RandomFigureFactory.hpp
        src/factory/stream_figure_factory/StreamFigureFactory.cpp
        src/factory/stream_figure_factory/StreamFigureFactory.hpp
        src/util/string_to_figure/StringToFigure.cpp
        src/util/string_to_figure/StringToFigure.hpp
        src/util/figure_util/FigureUtil.cpp
        src/util/figure_util/FigureUtil.hpp
        src/application/Application.cpp
        src/application/Application.hpp
        src/figure/Figure.cpp)
