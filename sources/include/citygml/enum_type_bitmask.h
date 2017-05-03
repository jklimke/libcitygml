#pragma once

#include <type_traits>
#include <istream>
#include <ostream>

enum class CityObjectsType2 : uint64_t {
	COT_GenericCityObject = 1ll,
	COT_Building = 1ll << 1,
	COT_Room = 1ll << 2,
	COT_BuildingInstallation = 1ll << 3,
	COT_BuildingFurniture = 1ll << 4,
	COT_Door = 1ll << 5,
	COT_Window = 1ll << 6,
	COT_CityFurniture = 1ll << 7,
	COT_Track = 1ll << 8,
	COT_Road = 1ll << 9,
	COT_Railway = 1ll << 10,
	COT_Square = 1ll << 11,
	COT_PlantCover = 1ll << 12,
	COT_SolitaryVegetationObject = 1ll << 13,
	COT_WaterBody = 1ll << 14,
	COT_ReliefFeature = 1ll << 15,
	COT_LandUse = 1ll << 16,
	COT_Tunnel = 1ll << 17,
	COT_Bridge = 1ll << 18,
	COT_BridgeConstructionElement = 1ll << 19,
	COT_BridgeInstallation = 1ll << 20,
	COT_BridgePart = 1ll << 21,
	COT_BuildingPart = 1ll << 22,

	COT_WallSurface = 1ll << 23,
	COT_RoofSurface = 1ll << 24,
	COT_GroundSurface = 1ll << 25,
	COT_ClosureSurface = 1ll << 26,
	COT_FloorSurface = 1ll << 27,
	COT_InteriorWallSurface = 1ll << 28,
	COT_CeilingSurface = 1ll << 29,
	COT_CityObjectGroup = 1ll << 30,
	COT_OuterCeilingSurface = 1ll << 31,
	COT_OuterFloorSurface = 1ll << 32,


	// covers all supertypes of tran::_TransportationObject that are not Track, Road, Railway or Square...
	// there are to many for to few bits to explicitly enumerate them. However Track, Road, Railway or Square should be used most of the time
	COT_TransportationObject = 1ll << 33,

	COT_All = 0xFFFFFFFFFFFFFFFFll
};

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


