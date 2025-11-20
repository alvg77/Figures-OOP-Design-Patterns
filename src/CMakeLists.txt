set(FIGURES_APPLICATION
        application/Application.cpp
        application/Application.hpp
)

set(FIGURES_FIGURE
        figure/Figure.hpp
        figure/Figure.cpp
        figure/triangle/Triangle.cpp
        figure/triangle/Triangle.hpp
        figure/rectangle/Rectangle.cpp
        figure/rectangle/Rectangle.hpp
        figure/circle/Circle.cpp
        figure/circle/Circle.hpp
)

set(FIGURES_UTIL
        util/Clonable.hpp
        util/StringConvertible.hpp
        util/string_to_figure/StringToFigure.cpp
        util/string_to_figure/StringToFigure.hpp
        util/figure_util/FigureUtil.cpp
        util/figure_util/FigureUtil.hpp
)

set(FIGURES_FACTORY
        factory/FigureFactory.hpp
        factory/abstract_factory/AbstractFactory.cpp
        factory/abstract_factory/AbstractFactory.hpp
        factory/random_figure_factory/RandomFigureFactory.cpp
        factory/random_figure_factory/RandomFigureFactory.hpp
        factory/stream_figure_factory/StreamFigureFactory.cpp
        factory/stream_figure_factory/StreamFigureFactory.hpp
)

add_library(figures_application ${FIGURES_APPLICATION})
add_library(figures_figure ${FIGURES_FIGURE})
add_library(figures_util ${FIGURES_UTIL})
add_library(figures_factory ${FIGURES_FACTORY})

target_link_libraries(figures_figure PRIVATE figures_util)
target_link_libraries(figures_util PRIVATE figures_figure)
target_link_libraries(figures_factory PRIVATE figures_figure figures_util)
target_link_libraries(figures_application PRIVATE figures_factory figures_figure figures_util)

add_executable(figures main.cpp)

target_link_libraries(figures
        figures_application
        figures_figure
        figures_util
        figures_factory
)
