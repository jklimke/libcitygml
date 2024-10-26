#pragma once

#include <type_traits>
#include <istream>
#include <ostream>

#include <citygml/citygml_api.h>

template <typename T>
class EnumClassBitmask
{
private:
    T t;

public:
    typedef typename std::underlying_type<T>::type underlying_type;

    /*constexpr*/ EnumClassBitmask() : t(T(0)) {}
    /*constexpr*/ EnumClassBitmask(T t) : t(t) {}
    /*constexpr*/ explicit EnumClassBitmask(underlying_type t) : t(T(t)) {}

    /*constexpr*/ /*explicit*/ operator bool() const { return bool(t); }
   /*constexpr*/ operator T() { return t; }

    /*constexpr*/ EnumClassBitmask operator|(T r) const { return EnumClassBitmask(t | r); }
    /*constexpr*/ EnumClassBitmask operator&(T r) const { return EnumClassBitmask(t & r); }
    /*constexpr*/ EnumClassBitmask operator^(T r) const { return EnumClassBitmask(t ^ r); }
    /*constexpr*/ EnumClassBitmask operator~() const { return EnumClassBitmask(~t); }

    const EnumClassBitmask& operator|=(T r) { t = t | r; return *this; }
    const EnumClassBitmask& operator&=(T r) { t = t & r; return *this; }
    const EnumClassBitmask& operator^=(T r) { t = t ^ r; return *this; }

   bool operator==(const EnumClassBitmask& r) { return underlying_type(t) == underlying_type(r.t); }
   bool operator==(const T& r) { return underlying_type(t) == underlying_type(r); }

   const EnumClassBitmask& setFromUnderlyingType(underlying_type value) { t = T(value); return *this; }

   friend std::istream& operator>> (std::istream &is, EnumClassBitmask& r) { underlying_type tmp;  is >> tmp; r.t = static_cast<T>(tmp);  return is; }
   friend std::ostream& operator<< (std::ostream &os, const EnumClassBitmask& r) { os << static_cast<underlying_type>(r.t); return os; }
};
