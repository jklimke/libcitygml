#include <citygml/vecs.hpp>

#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <limits>
#include <locale>
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

// TODO: Duplicated with FileReadTests.cpp
class LocaleModifier {
public:
    LocaleModifier(char const* newLocale) : previousLocale(std::locale::global(std::locale(newLocale))) {}

    ~LocaleModifier() {
        std::locale::global(previousLocale);
    }

private:
    std::locale const previousLocale;
};

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
    TVec2f floatTarget;
    TVec2d doubleTarget;

    constexpr std::array<TestData<2>, 17> TEST_STRINGS{
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
    };
    for (TestData<2> const& data : TEST_STRINGS) {
        floatTarget.fromString(data.sourceString);
        doubleTarget.fromString(data.sourceString);
        compareNumberOrNaN(floatTarget.x, data.expectedFloats[0]);
        compareNumberOrNaN(floatTarget.y, data.expectedFloats[1]);
        compareNumberOrNaN(doubleTarget.x, data.expectedDoubles[0]);
        compareNumberOrNaN(doubleTarget.y, data.expectedDoubles[1]);
    }

    constexpr std::array<std::string_view, 22> ERROR_STRINGS{
        "0,0 0,0"sv,
        "0 0,0"sv,
        "0, 0,"sv,
        "0 0,"sv,
        ",0 ,0"sv,
        "0 ,0"sv,
        "1.000,0 1.000,0"sv,
        "0 1.000,0"sv,
        "1,000.0 1,000.0"sv,
        "0 1,000.0"sv,
        "1'000.0 1'000.0"sv,
        "0 1'000.0"sv,
        "0.0.0 0.0.0"sv,
        "0 0.0.0"sv,
        ".0.0 .0.0"sv,
        "0 .0.0"sv,
        "5e 7e"sv,
        "5 7e"sv,
        "5e+ 7e+"sv,
        "5 7e+"sv,
        "5e- 7e-"sv,
        "5 7e-"sv,
    };
    for (std::string_view const& sourceString : ERROR_STRINGS) {
        EXPECT_THROW(floatTarget.fromString(sourceString), std::runtime_error);
        EXPECT_THROW(doubleTarget.fromString(sourceString), std::runtime_error);
    }
}

TEST(VecsTests, TVec3FromString) {
    TVec3f floatTarget;
    TVec3d doubleTarget;

    constexpr std::array<TestData<3>, 17> TEST_STRINGS{
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
    };
    for (TestData<3> const& data : TEST_STRINGS) {
        floatTarget.fromString(data.sourceString);
        doubleTarget.fromString(data.sourceString);
        compareNumberOrNaN(floatTarget.x, data.expectedFloats[0]);
        compareNumberOrNaN(floatTarget.y, data.expectedFloats[1]);
        compareNumberOrNaN(floatTarget.z, data.expectedFloats[2]);
        compareNumberOrNaN(doubleTarget.x, data.expectedDoubles[0]);
        compareNumberOrNaN(doubleTarget.y, data.expectedDoubles[1]);
        compareNumberOrNaN(doubleTarget.z, data.expectedDoubles[2]);
    }

    constexpr std::array<std::string_view, 11> ERROR_STRINGS{
        "0 0 0,0"sv,
        "0 0 0,"sv,
        "0 0 ,0"sv,
        "0 0 1.000,0"sv,
        "0 0 1,000.0"sv,
        "0 0 1'000.0"sv,
        "0 0 0.0.0"sv,
        "0 0 .0.0"sv,
        "0 0 9e"sv,
        "0 0 9e+"sv,
        "0 0 9e-"sv,
    };
    for (std::string_view const& sourceString : ERROR_STRINGS) {
        EXPECT_THROW(floatTarget.fromString(sourceString), std::runtime_error);
        EXPECT_THROW(doubleTarget.fromString(sourceString), std::runtime_error);
    }
}

TEST(VecsTests, TVec4FromString) {
    TVec4f floatTarget;
    TVec4d doubleTarget;

    constexpr std::array<TestData<4>, 17> TEST_STRINGS{
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
    };
    for (TestData<4> const& data : TEST_STRINGS) {
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

    constexpr std::array<std::string_view, 11> ERROR_STRINGS{
        "0 0 0 0,0"sv,
        "0 0 0 0,"sv,
        "0 0 0 ,0"sv,
        "0 0 0 1.000,0"sv,
        "0 0 0 1,000.0"sv,
        "0 0 0 1'000.0"sv,
        "0 0 0 0.0.0"sv,
        "0 0 0 .0.0"sv,
        "0 0 0 11e"sv,
        "0 0 0 11e+"sv,
        "0 0 0 11e-"sv,
    };
    for (std::string_view const& sourceString : ERROR_STRINGS) {
        EXPECT_THROW(floatTarget.fromString(sourceString), std::runtime_error);
        EXPECT_THROW(doubleTarget.fromString(sourceString), std::runtime_error);
    }
}

TEST(VecsTests, DifferentGlobalLocale) {
    LocaleModifier tempLocale("de_DE.UTF-8");

    TVec2f float2;
    float2.fromString("0.0 1.0");
    compareNumberOrNaN(float2.x, 0.f);
    compareNumberOrNaN(float2.y, 1.f);
    TVec3f float3;
    float3.fromString("0.0 1.0 2.0");
    compareNumberOrNaN(float3.z, 2.f);
    TVec4f float4;
    float4.fromString("0.0 1.0 2.0 3.0");
    compareNumberOrNaN(float4.w, 3.f);

    TVec2d double2;
    double2.fromString("0.0 1.0");
    compareNumberOrNaN(double2.x, 0.);
    compareNumberOrNaN(double2.y, 1.);
    TVec3d double3;
    double3.fromString("0.0 1.0 2.0");
    compareNumberOrNaN(double3.z, 2.);
    TVec4d double4;
    double4.fromString("0.0 1.0 2.0 3.0");
    compareNumberOrNaN(double4.w, 3.);

    EXPECT_THROW(float2.fromString("0,0 1,0"), std::runtime_error);
    EXPECT_THROW(double2.fromString("0,0 1,0"), std::runtime_error);
    EXPECT_THROW(float3.fromString("0,0 1,0 2,0"), std::runtime_error);
    EXPECT_THROW(double3.fromString("0,0 1,0 2,0"), std::runtime_error);
    EXPECT_THROW(float4.fromString("0,0 1,0 2,0 3,0"), std::runtime_error);
    EXPECT_THROW(double4.fromString("0,0 1,0 2,0 3,0"), std::runtime_error);
}
