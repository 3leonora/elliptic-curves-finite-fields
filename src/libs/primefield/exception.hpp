#ifndef _PRIMEFIELD_EXCEPTION_HPP
#define _PRIMEFIELD_EXCEPTION_HPP
//
// primefield/exception.hpp
//// Exception types used in the primefield package

#include <stdexcept>

namespace primefield {

class NotImplementedError: public std::runtime_error
{
    using runtime_error::runtime_error;
};


class NoInverseError : public std::range_error
{
    using range_error::range_error;
};

} // namespace primefield

#endif // _PRIMEFIELD_EXCEPTION_HPP