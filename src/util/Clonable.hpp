#ifndef FIGURES_CLONABLE_HPP
#define FIGURES_CLONABLE_HPP

class Clonable
{
  public:
    virtual Clonable *clone() const = 0;
    virtual ~Clonable() = default;
};

#endif // FIGURES_CLONABLE_HPP