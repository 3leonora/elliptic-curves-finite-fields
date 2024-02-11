#ifndef PRIMEFIELD_EXCEPTION_HPP
#define PRIMEFIELD_EXCEPTION_HPP
//
// primefield/exception.hpp
//// Exception types used in the primefield package

#include <stdexcept>

namespace primefield {

class NotImplementedError: public std::runtime_error
{
    using runtime_error::runtime_error;
};

class IterationError: public std::runtime_error
{
    using runtime_error::runtime_error;
};

class LiteralSyntax: public std::runtime_error
{
    using runtime_error::runtime_error;
};

class NoInverseError : public std::range_error
{
    using range_error::range_error;
};

} // namespace primefield

#endif // PRIMEFIELD_EXCEPTION_HPP
