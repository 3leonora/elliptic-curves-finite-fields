#ifndef PRIMEFIELD_TRAITS_HPP
#define PRIMEFIELD_TRAITS_HPP
//
// primefield/traits.hpp
//

#include <cstdint>

namespace primefield
{

  // Primary template
  template<typename T> class make_next_rank {};

  // Provide means to specify the next rank covering integer type.
  // e.g. uint32_t -> uint64_t

  // Helper type alias templates making it possible to have something like this in your code:
  //   using myhighuint_type = primefield::make_next_rank_t<myint_type>;
  template<typename T>
  using make_next_rank_t = typename make_next_rank<T>::utype;

  template<typename T>
  using make_next_signed_rank_t = typename make_next_rank<T>::stype;


  // Specialization for uint8_t
  template<>
  struct make_next_rank<uint8_t> {
    typedef uint16_t utype;
    typedef int16_t stype;
  };

  // Specialization for uint16_t
  template<>
  struct make_next_rank<uint16_t> {
    typedef uint32_t utype;
    typedef int32_t stype;
  };

  // Specialization for uint32_t
  template<>
  struct make_next_rank<uint32_t> {
    typedef uint64_t utype;
    typedef int64_t stype;
  };

  // Specialization for uint64_t
  template<>
  struct make_next_rank<uint64_t> {
    typedef unsigned __int128 utype;
    typedef __int128 stype;   // Will not mark as signed in traits it seems...
  };

} // namespace primefield

#endif // PRIMEFIELD_TRAITS_HPP
