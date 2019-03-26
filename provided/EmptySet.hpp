// EmptySet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A set implementation that is always empty.  This has value in our
// timing tests, where we want to factor out how much time is being
// spent inside our set implementations, as opposed to parsing input
// files or other tasks.

#ifndef EMPTYSET_HPP
#define EMPTYSET_HPP

#include "Set.hpp"



template <typename ElementType>
class EmptySet : public Set<ElementType>
{
public:
    virtual bool isImplemented() const noexcept override;
    virtual void add(const ElementType& element) override;
    virtual bool contains(const ElementType& element) const override;
    virtual unsigned int size() const noexcept override;
};


template <typename ElementType>
bool EmptySet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void EmptySet<ElementType>::add(const ElementType& element)
{
}


template <typename ElementType>
bool EmptySet<ElementType>::contains(const ElementType& element) const
{
    return false;
}


template <typename ElementType>
unsigned int EmptySet<ElementType>::size() const noexcept
{
    return 0;
}



#endif // EMPTYSET_HPP

