#include <citygml/citygml.h>
#include <citygml/citymodel.h>

#include <gtest/gtest.h>


TEST(AttributeParsingTests, JapaneseStringEncoding) {
    std::shared_ptr<const citygml::CityModel> city;
    try {
        citygml::ParserParams params;
        city = citygml::load("../../data/systemTests/AttributeParsing/japaneseCharacters.gml", params, nullptr);
    } catch(const std::runtime_error&){
        FAIL();
    }

    EXPECT_NE(city, nullptr);
    EXPECT_EQ(1u, city->getRootCityObjects().size());
    EXPECT_TRUE(city->getRootCityObjects()[0] != nullptr);
    citygml::CityObject const& building = *city->getRootCityObjects()[0];
    citygml::AttributesMap const& attrs = building.getAttributes();
    EXPECT_EQ(1u, attrs.size());
    auto const entry = attrs.find(u8"\u540D\u524D"); // TODO: When upgrading to C++20 the u8 literal will have type const char8_t
    EXPECT_NE(attrs.end(), entry);
    EXPECT_EQ(citygml::AttributeType::String, entry->second.getType());
    EXPECT_EQ(u8"\u75c5\u9662", entry->second.asString()); // TODO: When upgrading to C++20 the u8 literal will have type const char8_t
}