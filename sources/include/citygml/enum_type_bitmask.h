#pragma once

#include <type_traits>

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
};

#define ENUM_CLASS_BITWISE_OPERATORS(type_name) \
/*constexpr*/ type_name operator|(type_name l, type_name r) { return type_name(std::underlying_type<type_name>::type(l) | std::underlying_type<type_name>::type(r)); } \
/*constexpr*/ type_name operator&(type_name l, type_name r) { return type_name(std::underlying_type<type_name>::type(l) & std::underlying_type<type_name>::type(r)); } \
/*constexpr*/ type_name operator^(type_name l, type_name r) { return type_name(std::underlying_type<type_name>::type(l) ^ std::underlying_type<type_name>::type(r)); } \
/*constexpr*/ type_name operator~(type_name l) { return type_name(~std::underlying_type<type_name>::type(l)); }

#define ENUM_CLASS_BITWISE_OPERATORS_DEFS(type_name) \
/*constexpr*/ type_name operator|(type_name l, type_name r); \
/*constexpr*/ type_name operator&(type_name l, type_name r); \
/*constexpr*/ type_name operator^(type_name l, type_name r);\
/*constexpr*/ type_name operator~(type_name l);


