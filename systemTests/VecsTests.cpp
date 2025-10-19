#include <citygml/vecs.hpp>
#include <citygml/transformmatrix.h>
#include <parser/parserutils.hpp>

#include "GlobalLocaleSwitcher.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string_view>
using namespace std::literals;
using floatLimits = std::numeric_limits<float>;
using doubleLimits = std::numeric_limits<double>;

namespace {
	constexpr bool LOG = false;

void printIndent(unsigned int indent) {
	for ( unsigned int i = 0; i < indent; i++ ) std::cout << " ";
}

template <size_t N>
struct TestData {
    std::string_view sourceString;
    std::array<float, N> expectedFloats;
    std::array<double, N> expectedDoubles;

    constexpr TestData(std::string_view s,
                       std::array<float, N> f,
                       std::array<double, N> d)
        : sourceString(s), expectedFloats(f), expectedDoubles(d) {}
};

void compareNumber(float value, float expected) {
    EXPECT_FLOAT_EQ(value, expected);
}

void compareNumber(double value, double expected) {
    EXPECT_DOUBLE_EQ(value, expected);
}

template <typename T>
void compareNumberOrNaN(T value, T expected) {
    if (std::isnan(expected)) {
        EXPECT_TRUE(std::isnan(value));
    } else {
        compareNumber(value, expected);
    }
}
} // anonymous namespace

TEST(VecsTests, TVec2FromString) {
    constexpr std::array<TestData<2>, 41> TEST_STRINGS{
        TestData<2>{"0 0"sv, { 0.f, 0.f }, { 0., 0. }},
        {"1 -1"sv, { 1.f, -1.f }, { 1., -1. }},
        {"+1 -1"sv, { 1.f, -1.f }, { 1., -1. }},
        {"1. -1."sv, { 1.f, -1.f }, { 1., -1. }},
        {"+1. -1."sv, { 1.f, -1.f }, { 1., -1. }},
        {"1.0 -1.0"sv, { 1.f, -1.f }, { 1., -1. }},
        {"+1.0 -1.0"sv, { 1.f, -1.f }, { 1., -1. }},
        {"0.1 -0.1"sv, { 0.1f, -0.1f }, { 0.1, -0.1 }},
        {"+0.1 -0.1"sv, { 0.1f, -0.1f }, { 0.1, -0.1 }},
        {"1.1e0 -1.1e0"sv, { 1.1f, -1.1f }, { 1.1, -1.1 }},
        {"+1.1e0 -1.1e0"sv, { 1.1f, -1.1f }, { 1.1, -1.1 }},
        {"1.1e10 -1.1e10"sv, { 1.1e10f, -1.1e10f }, { 1.1e10, -1.1e10 }},
        {"+1.1e10 -1.1e10"sv, { 1.1e10f, -1.1e10f }, { 1.1e10, -1.1e10 }},
        {"1.1e-10 -1.1e-10"sv, { 1.1e-10f, -1.1e-10f }, { 1.1e-10, -1.1e-10 }},
        {"+1.1e-10 -1.1e-10"sv, { 1.1e-10f, -1.1e-10f }, { 1.1e-10, -1.1e-10 }},
        {"INF -INF"sv, { floatLimits::infinity(), -floatLimits::infinity() }, { doubleLimits::infinity(), -doubleLimits::infinity() }},
        {"NaN -NaN"sv, { floatLimits::quiet_NaN(), floatLimits::quiet_NaN() }, { doubleLimits::quiet_NaN(), doubleLimits::quiet_NaN() }},
        // Unexpected inputs - document expected behavior
        {"1,2 3,4"sv, { 1.f, 0.f }, { 1., 0.}},
        {"1 2,3"sv, { 1.f, 2.f }, { 1., 2. }},
        {"1, 2,"sv, { 1.f, 0.f }, { 1., 0.}},
        {"1 2,"sv, { 1.f, 2.f }, { 1., 2. }},
        {",1 ,2"sv, { 0.f, 0.f }, { 0., 0.}},
        {"1 ,2"sv, { 1.f, 0.f }, { 1., 0. }},
        {"1.200,3 4.500,6"sv, { 1.2f, 0.f }, { 1.2, 0. }},
        {"1 2.300,4"sv, { 1.f, 2.3f }, { 1., 2.3 }},
        {"1,200.3 4,500.6"sv, { 1.f, 0.f }, { 1., 0. }},
        {"1 2,300.4"sv, { 1.f, 2.f }, { 1., 2. }},
        {"1'200.3 4'500.6"sv, { 1.f, 0.f }, { 1., 0. }},
        {"1 2'300.4"sv, { 1.f, 2.f }, { 1., 2. }},
        {"1.2.3 4.5.6"sv, { 1.2f, 0.3f }, { 1.2, 0.3 }},
        {"1 2.3.4"sv, { 1.f, 2.3f }, { 1., 2.3 }},
        {".1.2 .3.4"sv, { 0.1f, 0.2f }, { 0.1, 0.2 }},
        {"1 .2.3"sv, { 1.f, 0.2f }, { 1., 0.2 }},
        {"5e 7e"sv, { 5.f, 0.f }, { 5, 0. }},
        {"5 7e"sv, { 5.f, 7.f }, { 5., 7. }},
        {"5e+ 7e+"sv, { 5.f, 0.f }, { 5, 0. }},
        {"5 7e+"sv, { 5.f, 7.f }, { 5., 7. }},
        {"5e- 7e-"sv, { 5.f, 0.f }, { 5, 0. }},
        {"5 7e-"sv, { 5.f, 7.f }, { 5., 7. }},
        {"1   "sv, { 1.f, 0.f }, { 1., 0. }},
        {"   "sv, { 0.f, 0.f }, { 0., 0. }},
    };
    for (TestData<2> const& data : TEST_STRINGS) {
        TVec2f floatTarget;
        TVec2d doubleTarget;
        floatTarget.fromString(data.sourceString);
        doubleTarget.fromString(data.sourceString);

        compareNumberOrNaN(floatTarget.x, data.expectedFloats[0]);
        compareNumberOrNaN(floatTarget.y, data.expectedFloats[1]);
        compareNumberOrNaN(doubleTarget.x, data.expectedDoubles[0]);
        compareNumberOrNaN(doubleTarget.y, data.expectedDoubles[1]);
    }
}

TEST(VecsTests, TVec3FromString) {
    constexpr std::array<TestData<3>, 30> TEST_STRINGS{
        TestData<3>{"0 0 0"sv, { 0.f, 0.f, 0.f }, { 0., 0., 0. }},
        {"1 -1 2"sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"+1 -1 +2"sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"1. -1. 2."sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"+1. -1. +2."sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"1.0 -1.0 2.0"sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"+1.0 -1.0 +2.0"sv, { 1.f, -1.f, 2.f }, { 1., -1., 2. }},
        {"0.1 -0.1 0.2"sv, { 0.1f, -0.1f, 0.2f }, { 0.1, -0.1, 0.2 }},
        {"+0.1 -0.1 +0.2"sv, { 0.1f, -0.1f, 0.2f }, { 0.1, -0.1, 0.2 }},
        {"1.1e0 -1.1e0 2.2e0"sv, { 1.1f, -1.1f, 2.2f }, { 1.1, -1.1, 2.2 }},
        {"+1.1e0 -1.1e0 +2.2e0"sv, { 1.1f, -1.1f, 2.2f }, { 1.1, -1.1, 2.2 }},
        {"1.1e10 -1.1e10 2.2e15"sv, { 1.1e10f, -1.1e10f, 2.2e15f }, { 1.1e10, -1.1e10, 2.2e15 }},
        {"+1.1e10 -1.1e10 +2.2e15"sv, { 1.1e10f, -1.1e10f, 2.2e15f }, { 1.1e10, -1.1e10, 2.2e15 }},
        {"1.1e-10 -1.1e-10 2.2e-15"sv, { 1.1e-10f, -1.1e-10f, 2.2e-15f }, { 1.1e-10, -1.1e-10, 2.2e-15 }},
        {"+1.1e-10 -1.1e-10 +2.2e-15"sv, { 1.1e-10f, -1.1e-10f, 2.2e-15f }, { 1.1e-10, -1.1e-10, 2.2e-15 }},
        {"1.0 -1.0 INF"sv, { 1.f, -1.f, floatLimits::infinity() }, { 1., -1., doubleLimits::infinity() }},
        {"1.0 -1.0 NaN"sv, { 1.f, -1.f, floatLimits::quiet_NaN() }, { 1., -1., doubleLimits::quiet_NaN() }},
        // Unexpected inputs - document expected behavior
        {"1 2 3,4"sv, { 1.f, 2.f, 3.f }, { 1., 2., 3. }},
        {"1 2 3,"sv, { 1.f, 2.f, 3.f }, { 1., 2., 3. }},
        {"1 2 ,3"sv, { 1.f, 2.f, 0.f }, { 1., 2., 0. }},
        {"1 2 3.400,5"sv, { 1.f, 2.f, 3.4f }, { 1., 2., 3.4 }},
        {"1 2 3,400.5"sv, { 1.f, 2.f, 3.f }, { 1., 2., 3. }},
        {"1 2 3'400.5"sv, { 1.f, 2.f, 3.f }, { 1., 2., 3. }},
        {"1 2 3.4.5"sv, { 1.f, 2.f, 3.4f }, { 1., 2., 3.4 }},
        {"1 2 .3.4"sv, { 1.f, 2.f, 0.3f }, { 1., 2., 0.3 }},
        {"1 2 5e"sv, { 1.f, 2.f, 5.f }, { 1., 2., 5. }},
        {"1 2 5e+"sv, { 1.f, 2.f, 5.f }, { 1., 2., 5. }},
        {"1 2 5e-"sv, { 1.f, 2.f, 5.f }, { 1., 2., 5. }},
        {"1 2   "sv, { 1.f, 2.f, 0.f }, { 1., 2., 0. }},
        {"   "sv, { 0.f, 0.f, 0.f }, { 0., 0., 0. }},
    };
    for (TestData<3> const& data : TEST_STRINGS) {
        TVec3f floatTarget;
        TVec3d doubleTarget;
        floatTarget.fromString(data.sourceString);
        doubleTarget.fromString(data.sourceString);

        compareNumberOrNaN(floatTarget.x, data.expectedFloats[0]);
        compareNumberOrNaN(floatTarget.y, data.expectedFloats[1]);
        compareNumberOrNaN(floatTarget.z, data.expectedFloats[2]);
        compareNumberOrNaN(doubleTarget.x, data.expectedDoubles[0]);
        compareNumberOrNaN(doubleTarget.y, data.expectedDoubles[1]);
        compareNumberOrNaN(doubleTarget.z, data.expectedDoubles[2]);
    }
}

TEST(VecsTests, TVec4FromString) {
    constexpr std::array<TestData<4>, 30> TEST_STRINGS{
        TestData<4>{"0 0 0 0"sv, { 0.f, 0.f, 0.f, 0.f }, { 0., 0., 0., 0. }},
        {"1 -1 2 -2"sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"+1 -1 +2 -2"sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"1. -1. 2. -2."sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"+1. -1. +2. -2."sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"1.0 -1.0 2.0 -2.0"sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"+1.0 -1.0 +2.0 -2.0"sv, { 1.f, -1.f, 2.f, -2.f }, { 1., -1., 2., -2. }},
        {"0.1 -0.1 0.2 -0.2"sv, { 0.1f, -0.1f, 0.2f, -0.2f }, { 0.1, -0.1, 0.2, -0.2 }},
        {"+0.1 -0.1 +0.2 -0.2"sv, { 0.1f, -0.1f, 0.2f, -0.2f }, { 0.1, -0.1, 0.2, -0.2 }},
        {"1.1e0 -1.1e0 2.2e0 -2.2e0"sv, { 1.1f, -1.1f, 2.2f, -2.2f }, { 1.1, -1.1, 2.2, -2.2 }},
        {"+1.1e0 -1.1e0 +2.2e0 -2.2e0"sv, { 1.1f, -1.1f, 2.2f, -2.2f }, { 1.1, -1.1, 2.2, -2.2 }},
        {"1.1e10 -1.1e10 2.2e15 -2.2e15"sv, { 1.1e10f, -1.1e10f, 2.2e15f, -2.2e15f }, { 1.1e10, -1.1e10, 2.2e15, -2.2e15 }},
        {"+1.1e10 -1.1e10 +2.2e15 -2.2e15"sv, { 1.1e10f, -1.1e10f, 2.2e15f, -2.2e15f }, { 1.1e10, -1.1e10, 2.2e15, -2.2e15 }},
        {"1.1e-10 -1.1e-10 2.2e-15 -2.2e-15"sv, { 1.1e-10f, -1.1e-10f, 2.2e-15f, -2.2e-15f }, { 1.1e-10, -1.1e-10, 2.2e-15, -2.2e-15 }},
        {"+1.1e-10 -1.1e-10 +2.2e-15 -2.2e-15"sv, { 1.1e-10f, -1.1e-10f, 2.2e-15f, -2.2e-15f }, { 1.1e-10, -1.1e-10, 2.2e-15, -2.2e-15 }},
        {"1.0 -1.0 1.0 -INF"sv, { 1.f, -1.f, 1.f, -floatLimits::infinity() }, { 1., -1., 1., -doubleLimits::infinity() }},
        {"1.0 -1.0 1.0 -NaN"sv, { 1.f, -1.f, 1.f, floatLimits::quiet_NaN() }, { 1., -1., 1., doubleLimits::quiet_NaN() }},
        // Unexpected inputs - document expected behavior
        {"1 2 3 4,5"sv, { 1.f, 2.f, 3.f, 4.f }, { 1., 2., 3., 4. }},
        {"1 2 3 4,"sv, { 1.f, 2.f, 3.f, 4.f }, { 1., 2., 3., 4. }},
        {"1 2 3 ,4"sv, { 1.f, 2.f, 3.f, 0.f }, { 1., 2., 3., 0. }},
        {"1 2 3 4.500,6"sv, { 1.f, 2.f, 3.f, 4.5f }, { 1., 2., 3., 4.5 }},
        {"1 2 3 4,500.6"sv, { 1.f, 2.f, 3.f, 4.f }, { 1., 2., 3., 4. }},
        {"1 2 3 4'500.6"sv, { 1.f, 2.f, 3.f, 4.f }, { 1., 2., 3., 4. }},
        {"1 2 3 4.5.6"sv, { 1.f, 2.f, 3.f, 4.5f }, { 1., 2., 3., 4.5 }},
        {"1 2 3 .4.5"sv, { 1.f, 2.f, 3.f, 0.4f }, { 1., 2., 3., 0.4 }},
        {"1 2 3 5e"sv, { 1.f, 2.f, 3.f, 5.f }, { 1., 2., 3., 5. }},
        {"1 2 3 5e+"sv, { 1.f, 2.f, 3.f, 5.f }, { 1., 2., 3., 5. }},
        {"1 2 3 5e-"sv, { 1.f, 2.f, 3.f, 5.f }, { 1., 2., 3., 5. }},
        {"1 2 3   "sv, { 1.f, 2.f, 3.f, 0.f }, { 1., 2., 3., 0. }},
        {"   "sv, { 0.f, 0.f, 0.f, 0.f }, { 0., 0., 0., 0. }},
    };
    for (TestData<4> const& data : TEST_STRINGS) {
        TVec4f floatTarget;
        TVec4d doubleTarget;
        floatTarget.fromString(data.sourceString);
        doubleTarget.fromString(data.sourceString);

        compareNumberOrNaN(floatTarget.x, data.expectedFloats[0]);
        compareNumberOrNaN(floatTarget.y, data.expectedFloats[1]);
        compareNumberOrNaN(floatTarget.z, data.expectedFloats[2]);
        compareNumberOrNaN(floatTarget.w, data.expectedFloats[3]);
        compareNumberOrNaN(doubleTarget.x, data.expectedDoubles[0]);
        compareNumberOrNaN(doubleTarget.y, data.expectedDoubles[1]);
        compareNumberOrNaN(doubleTarget.z, data.expectedDoubles[2]);
        compareNumberOrNaN(doubleTarget.w, data.expectedDoubles[3]);
    }
}

TEST(VecsTests, DifferentGlobalLocale) {
    test::GlobalLocaleSwitcher const tempLocale("de_DE.UTF-8");

    TVec2f float2;
    float2.fromString("0.0 1.0");
    compareNumberOrNaN(float2.x, 0.f);
    compareNumberOrNaN(float2.y, 1.f);
    float2.fromString("1,0 2,0");
    compareNumberOrNaN(float2.x, 1.f);
    compareNumberOrNaN(float2.y, 0.f);
    TVec3f float3;
    float3.fromString("0.0 1.0 2.0");
    compareNumberOrNaN(float3.z, 2.f);
    float3.fromString("0 1 3,0");
    compareNumberOrNaN(float3.z, 3.f);
    TVec4f float4;
    float4.fromString("0.0 1.0 2.0 3.0");
    compareNumberOrNaN(float4.w, 3.f);
    float4.fromString("0 1 2 4,0");
    compareNumberOrNaN(float4.w, 4.f);

    TVec2d double2;
    double2.fromString("0.0 1.0");
    compareNumberOrNaN(double2.x, 0.);
    compareNumberOrNaN(double2.y, 1.);
    double2.fromString("1,0 2,0");
    compareNumberOrNaN(double2.x, 1.);
    compareNumberOrNaN(double2.y, 0.);
    TVec3d double3;
    double3.fromString("0.0 1.0 2.0");
    compareNumberOrNaN(double3.z, 2.);
    double3.fromString("0 1 3,0");
    compareNumberOrNaN(double3.z, 3.);
    TVec4d double4;
    double4.fromString("0.0 1.0 2.0 3.0");
    compareNumberOrNaN(double4.w, 3.);
    double4.fromString("0 1 2 4,0");
    compareNumberOrNaN(double4.w, 4.);
}

namespace {
    std::string const FILE_NAME = "testFile.gml";
    class TestDocumentLocation : public citygml::DocumentLocation {
    public:
        const std::string& getDocumentFileName() const override { return FILE_NAME; }
        uint64_t getCurrentLine() const override { return 7; }
        uint64_t getCurrentColumn() const override { return 42; }
    };
    
    struct MatrixTestData {
        std::string_view sourceString;
        std::array<double, 16> expectedDoubles;

        MatrixTestData(std::string_view s, std::array<double, 16> matcher)
            : sourceString(s), expectedDoubles(matcher) {}
    };

    class MemoryLogger : public citygml::CityGMLLogger {
    private:
        mutable std::string allLogs;
    public:
        MemoryLogger(LOGLEVEL level = LOGLEVEL::LL_ERROR) : citygml::CityGMLLogger(level) {}
        void log(LOGLEVEL level, const std::string& message, const char* file=nullptr, int line=-1) const override {
            allLogs += "Level: " + std::to_string(static_cast<size_t>(level)) + ": " + (file != nullptr ? file : "") + (line != -1 ? ":" + std::to_string(line) : "") + ": " + message + "\n";
        }
        std::string getLogsAndClear() {
            return { std::move(allLogs) };
        }
    };

} // anonymous namespace

TEST(VecsTests, Matrix) {
    const std::array<MatrixTestData, 15> TEST_STRINGS{
        MatrixTestData{"0 -1 1 +1  0. -1. 1. +1.  0.0 -1.0 1.0 +1.0  0.00 -0.1 0.1 +0.1"sv, { 0., -1., 1., 1.,  0., -1., 1., 1.,  0., -1., 1., 1.,  0., -0.1, 0.1, 0.1 } },
        MatrixTestData{"0.0e0 -1.1e0 1.1e0 +1.1e0  0.0e10 -1.1e10 1.1e10 +1.1e10  0.0e-10 -1.1e-10 1.1e-10 +1.1e-10  0.0e+10 -1.1e+10 1.1e+10 +1.1e+10"sv, { 0., -1.1, 1.1, 1.1,  0., -1.1e10, 1.1e10, 1.1e10,  0., -1.1e-10, 1.1e-10, 1.1e-10,  0., -1.1e10, 1.1e10, 1.1e10 } },
        MatrixTestData{"INF -INF +INF 0  0 1 0 0  0 0 1 0  0 0 0 1"sv, { doubleLimits::infinity(), -doubleLimits::infinity(), doubleLimits::infinity(), 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        // Unexpected inputs - document expected behavior
        MatrixTestData{"2,1     4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2,      4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{",2      4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 1., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2.100,3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2.1, 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2,100.3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2'100.3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2.1.3   4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2.1, 0.3, 4., 5.,  6., 7., 8., 9.,  10., 11., 12., 13.,  14., 15., 16., 17. } },
        MatrixTestData{".2.1    4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 0.2, 0.1, 4., 5.,  6., 7., 8., 9.,  10., 11., 12., 13.,  14., 15., 16., 17. } },
        MatrixTestData{"2e      4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2e+     4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"2e-     4 5 6 7 8 9 10 11 12 13 14 15 16 17 18", { 2., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
        MatrixTestData{"   ", { 1., 0., 0., 0.,  0., 1., 0., 0.,  0., 0., 1., 0.,  0., 0., 0., 1. } },
    };
    std::shared_ptr<citygml::CityGMLLogger> noLogger;
    TestDocumentLocation loc;

    for (MatrixTestData const& data : TEST_STRINGS) {
        citygml::TransformationMatrix matrix = parseMatrix(std::string(data.sourceString), noLogger, loc);
        std::vector<double> matrixCopy(matrix.getMatrix(), matrix.getMatrix() + 16);
        ASSERT_THAT(matrixCopy, testing::ElementsAreArray(data.expectedDoubles));
    }

    citygml::TransformationMatrix nanMatrix = parseMatrix(std::string("NaN -NaN +NaN 0  0 1 0 0  0 0 1 0  0 0 0 1"sv), noLogger, loc);
    ASSERT_TRUE(std::isnan(nanMatrix.getMatrix()[0]));
    ASSERT_TRUE(std::isnan(nanMatrix.getMatrix()[1]));
    ASSERT_TRUE(std::isnan(nanMatrix.getMatrix()[2]));

    auto logger = std::make_shared<MemoryLogger>(citygml::CityGMLLogger::LOGLEVEL::LL_WARNING);
    auto citygmlLogger = std::static_pointer_cast<citygml::CityGMLLogger>(logger);
    citygml::TransformationMatrix matrix = parseMatrix(std::string("0 0 0 1"sv), citygmlLogger, loc);
    std::string const logs = logger->getLogsAndClear();
    std::stringstream locatorString;
    locatorString << loc;
    EXPECT_THAT(logs, testing::StartsWith("Level: 3")); // Checks that log level is Warning
    EXPECT_THAT(logs, testing::HasSubstr("Matrix with 16 elements expected, got '5' at " + locatorString.str() + ". Matrix may be invalid."));
}
