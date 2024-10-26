#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>

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
    EXPECT_EQ(CityObjectsTypeMask(CityObject::CityObjectsType::COT_All), params.objectsMask);

    params.objectsMask = CityObject::CityObjectsType::COT_All;
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

// TODO: Make masks const - prohibited by non-const equality operator.
TEST(CityObjectsTypeMask, CityObjectTypeBitwiseOperations) {
    CityObjectsTypeMask const defaultMask;
    EXPECT_EQ(0, defaultMask);

    CityObjectsTypeMask /*const*/ fullMask(CityObject::CityObjectsType::COT_All);
    EXPECT_TRUE(fullMask == CityObject::CityObjectsType::COT_All);

    CityObjectsTypeMask /*const*/ emptyMask(~CityObject::CityObjectsType::COT_All);
    EXPECT_EQ(emptyMask, 0);

    CityObjectsTypeMask /*const*/ buildingMask(CityObject::CityObjectsType::COT_All & CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(buildingMask == CityObject::CityObjectsType::COT_Building);

    CityObjectsTypeMask /*const*/ buildingDoorMask(CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Door);
    EXPECT_TRUE(buildingDoorMask == static_cast<CityObject::CityObjectsType>(34));

    CityObjectsTypeMask /*const*/ notBuildingMask(CityObject::CityObjectsType::COT_All ^ CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(notBuildingMask == ~CityObject::CityObjectsType::COT_Building);
}

// TODO: Make masks const - prohibited by non-const equality operator.
TEST(CityObjectsTypeMask, CityObjectTypeMaskBitwiseOperations) {
    CityObjectsTypeMask const defaultMask;
    EXPECT_EQ(0, defaultMask);

    CityObjectsTypeMask /*const*/ fullMask = CityObjectsTypeMask(CityObject::CityObjectsType::COT_All);
    EXPECT_TRUE(fullMask == CityObject::CityObjectsType::COT_All);

    CityObjectsTypeMask const emptyMask = ~CityObjectsTypeMask(CityObject::CityObjectsType::COT_All);
    EXPECT_EQ(emptyMask, 0);

    CityObjectsTypeMask /*const*/ buildingMask = CityObjectsTypeMask(CityObject::CityObjectsType::COT_All) & CityObjectsTypeMask(CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(buildingMask == CityObject::CityObjectsType::COT_Building);

    CityObjectsTypeMask /*const*/ buildingDoorMask = CityObjectsTypeMask(CityObject::CityObjectsType::COT_Building) | CityObjectsTypeMask(CityObject::CityObjectsType::COT_Door);
    EXPECT_TRUE(buildingDoorMask == static_cast<CityObject::CityObjectsType>(34));

    CityObjectsTypeMask /*const*/ notBuildingMask = CityObjectsTypeMask(CityObject::CityObjectsType::COT_All) ^ CityObjectsTypeMask(CityObject::CityObjectsType::COT_Building);
    EXPECT_TRUE(notBuildingMask == ~CityObject::CityObjectsType::COT_Building);

    CityObjectsTypeMask assignMask;
    assignMask |= CityObject::CityObjectsType::COT_All;
    EXPECT_TRUE(assignMask == CityObject::CityObjectsType::COT_All);
    assignMask ^= CityObject::CityObjectsType::COT_Building;
    EXPECT_TRUE(assignMask == ~CityObject::CityObjectsType::COT_Building);
    assignMask &= CityObject::CityObjectsType::COT_Door;
    EXPECT_TRUE(assignMask == CityObject::CityObjectsType::COT_Door);

    CityObjectsTypeMask mixMaskLeft;
    mixMaskLeft = mixMaskLeft | CityObject::CityObjectsType::COT_All;
    EXPECT_TRUE(mixMaskLeft == CityObject::CityObjectsType::COT_All);
    mixMaskLeft = mixMaskLeft ^ CityObject::CityObjectsType::COT_Building;
    EXPECT_TRUE(mixMaskLeft == ~CityObject::CityObjectsType::COT_Building);
    mixMaskLeft = mixMaskLeft & CityObject::CityObjectsType::COT_Door;
    EXPECT_TRUE(mixMaskLeft == CityObject::CityObjectsType::COT_Door);

    CityObjectsTypeMask mixMaskRight;
    mixMaskRight = CityObject::CityObjectsType::COT_All | mixMaskRight;
    EXPECT_TRUE(mixMaskRight == CityObject::CityObjectsType::COT_All);
    mixMaskRight = CityObject::CityObjectsType::COT_Building ^ mixMaskRight;
    EXPECT_TRUE(mixMaskRight == ~CityObject::CityObjectsType::COT_Building);
    mixMaskRight = CityObject::CityObjectsType::COT_Door & mixMaskLeft;
    EXPECT_TRUE(mixMaskRight == CityObject::CityObjectsType::COT_Door);
}

TEST(CityObjectsTypeMask, FilterStreamInterface) {
    std::stringstream inStream("34", std::ios_base::in);
    CityObjectsTypeMask inMask;
    inStream >> inMask;
    EXPECT_TRUE(inMask == (CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Door));

    std::stringstream outStream(std::ios_base::out);
    CityObjectsTypeMask outMask(CityObject::CityObjectsType::COT_Building | CityObject::CityObjectsType::COT_Door);
    outStream << outMask;
    EXPECT_TRUE(outStream.str() == "34");
}
