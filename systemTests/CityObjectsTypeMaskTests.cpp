#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/warnings.h>

#include <gtest/gtest.h>

#include <sstream>


namespace {
using CityModelPtr = std::shared_ptr<citygml::CityModel const>;

inline void readBridgeAndBuildingWithParams(citygml::ParserParams const& params, size_t expectedCityObjectCount) {
    CityModelPtr city = citygml::load("../../data/systemTests/CityObjectsTypeMask/BridgeAndBuilding.gml", params, nullptr);
    EXPECT_EQ(expectedCityObjectCount, city->getNumRootCityObjects());
}

} // anonymous namespace

using namespace citygml;

TEST(CityObjectsTypeMask, FilterBuilding) {
    ParserParams params;
    EXPECT_EQ(toMask(CityObject::CityObjectsType::COT_All), params.objectsMask);


    // This is intentionally calling the deprecated assignment to test/verify that it works.
    PRAGMA_NO_DEPRECATION_BEGIN
    params.objectsMask = CityObject::CityObjectsType::COT_All;
    PRAGMA_NO_DEPRECATION_END
    readBridgeAndBuildingWithParams(params, 2);

    params.objectsMask = CityObject::CityObjectsType::COT_Door & CityObject::CityObjectsType::COT_Building;
    readBridgeAndBuildingWithParams(params, 0);

    params.objectsMask = CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Road;
    readBridgeAndBuildingWithParams(params, 1);

    params.objectsMask = CityObject::CityObjectsType::COT_All ^ CityObject::CityObjectsType::COT_Building;
    readBridgeAndBuildingWithParams(params, 1);

    params.objectsMask = ~CityObject::CityObjectsType::COT_Building;
    readBridgeAndBuildingWithParams(params, 1);
}

TEST(CityObjectsTypeMask, CityObjectTypeBitwiseOperations) {
    CityObjectsTypeMask const defaultMask;
    EXPECT_EQ(0, defaultMask.count());

    CityObjectsTypeMask const fullMask(toMask(CityObject::CityObjectsType::COT_All));
    EXPECT_TRUE(fullMask == toMask(CityObject::CityObjectsType::COT_All));
    EXPECT_EQ(fullMask.count(), static_cast<size_t>(CityObject::CityObjectsType::COT_All));

    CityObjectsTypeMask const emptyMask(~CityObject::CityObjectsType::COT_All);
    EXPECT_EQ(emptyMask, 0);

    CityObjectsTypeMask const buildingMask(CityObject::CityObjectsType::COT_All & CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(buildingMask == toMask(CityObject::CityObjectsType::COT_Building));
    EXPECT_EQ(buildingMask.count(), 1);
    EXPECT_TRUE(buildingMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Building)));

    CityObjectsTypeMask const buildingDoorMask(CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Door);
    EXPECT_EQ(buildingDoorMask.count(), 2);
    EXPECT_TRUE(buildingDoorMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Building)));
    EXPECT_TRUE(buildingDoorMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Door)));

    CityObjectsTypeMask const notBuildingMask(CityObject::CityObjectsType::COT_All ^ CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(notBuildingMask == ~CityObject::CityObjectsType::COT_Building);
}

TEST(CityObjectsTypeMask, CityObjectTypeMaskBitwiseOperations) {
    CityObjectsTypeMask const defaultMask;
    EXPECT_EQ(0, defaultMask.count());

    CityObjectsTypeMask const fullMask = toMask(CityObject::CityObjectsType::COT_All);
    EXPECT_TRUE(fullMask == toMask(CityObject::CityObjectsType::COT_All));
    EXPECT_EQ(fullMask.count(), static_cast<size_t>(CityObject::CityObjectsType::COT_All));

    CityObjectsTypeMask const emptyMask = ~toMask(CityObject::CityObjectsType::COT_All);
    EXPECT_EQ(emptyMask, 0);

    CityObjectsTypeMask const buildingMask = toMask(CityObject::CityObjectsType::COT_All) & toMask(CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(buildingMask == toMask(CityObject::CityObjectsType::COT_Building));
    EXPECT_EQ(buildingMask.count(), 1);
    EXPECT_TRUE(buildingMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Building)));

    CityObjectsTypeMask const buildingDoorMask = toMask(CityObject::CityObjectsType::COT_Building) | toMask(CityObject::CityObjectsType::COT_Door);
    EXPECT_EQ(buildingDoorMask.count(), 2);
    EXPECT_TRUE(buildingDoorMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Building)));
    EXPECT_TRUE(buildingDoorMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Door)));

    CityObjectsTypeMask const notBuildingMask = toMask(CityObject::CityObjectsType::COT_All) ^ toMask(CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(notBuildingMask == ~CityObject::CityObjectsType::COT_Building);

    CityObjectsTypeMask assignMask;
    assignMask |= toMask(CityObject::CityObjectsType::COT_All);
    EXPECT_EQ(assignMask.count(), static_cast<size_t>(CityObject::CityObjectsType::COT_All));
    assignMask ^= toMask(CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(assignMask == ~CityObject::CityObjectsType::COT_Building);
    assignMask &= toMask(CityObject::CityObjectsType::COT_Door);
    EXPECT_TRUE(assignMask == toMask(CityObject::CityObjectsType::COT_Door));

    CityObjectsTypeMask mixMaskLeft;
    mixMaskLeft = mixMaskLeft | CityObject::CityObjectsType::COT_All;
    EXPECT_EQ(mixMaskLeft.count(), static_cast<size_t>(CityObject::CityObjectsType::COT_All));
    mixMaskLeft = mixMaskLeft ^ CityObject::CityObjectsType::COT_Building;
    EXPECT_TRUE(mixMaskLeft == ~CityObject::CityObjectsType::COT_Building);
    mixMaskLeft = mixMaskLeft & CityObject::CityObjectsType::COT_Door;
    EXPECT_TRUE(mixMaskLeft == toMask(CityObject::CityObjectsType::COT_Door));

    CityObjectsTypeMask mixMaskRight;
    mixMaskRight = CityObject::CityObjectsType::COT_All | mixMaskRight;
    EXPECT_EQ(mixMaskRight.count(), static_cast<size_t>(CityObject::CityObjectsType::COT_All));
    mixMaskRight = CityObject::CityObjectsType::COT_Building ^ mixMaskRight;
    EXPECT_TRUE(mixMaskRight == ~CityObject::CityObjectsType::COT_Building);
    mixMaskRight = CityObject::CityObjectsType::COT_Door & mixMaskLeft;
    EXPECT_TRUE(mixMaskRight == toMask(CityObject::CityObjectsType::COT_Door));
}

TEST(CityObjectsTypeMask, FilterStreamInterface) {
    std::stringstream inStream("100010", std::ios_base::in);
    CityObjectsTypeMask inMask;
    inStream >> inMask;
    EXPECT_EQ(inMask.count(), 2);
    EXPECT_TRUE(inMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Building)));
    EXPECT_TRUE(inMask.test(static_cast<size_t>(CityObject::CityObjectsType::COT_Door)));

    std::stringstream outStream(std::ios_base::out);
    CityObjectsTypeMask outMask(CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Door);
    outStream << outMask;
    EXPECT_EQ(outStream.str(), "000000000000000000000000000000000000000000000000000000100010");
}
