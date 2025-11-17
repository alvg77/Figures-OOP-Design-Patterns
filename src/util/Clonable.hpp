#ifndef FIGURES_CLONABLE_HPP
#define FIGURES_CLONABLE_HPP
#include <memory>

class Clonable {
public:
    virtual Clonable *clone() const = 0;
    virtual ~Clonable() = default;
};

#endif //FIGURES_CLONABLE_HPP