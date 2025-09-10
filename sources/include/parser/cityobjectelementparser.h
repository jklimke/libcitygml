#pragma once

#include <parser/gmlfeaturecollectionparser.h>

#include <citygml/cityobject.h>

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <mutex>

#include <citygml/cityobject.h>

namespace citygml {

    class CityObjectElementParser : public GMLFeatureCollectionElementParser {
    public:
        CityObjectElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(CityObject*)> callback);

        // ElementParser interface
        std::string elementParserName() const override;
        bool handlesElement(const NodeType::XMLNode &node) const override;
    protected:

        // CityGMLElementParser interface
        bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;
        bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes) override;
        bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters) override;

        // GMLFeatureCollectionElementParser interface
        FeatureObject* getFeatureObject() override;

    private:
        static void initializeTypeIDTypeMap();
        static void initializeAttributesSet();
        static AttributeType getAttributeType(const NodeType::XMLNode& node);

        CityObject* m_model;
        std::function<void(CityObject*)> m_callback;
        AttributeType m_lastGenericAttributeType;
        std::string m_lastGenericAttributeValue;
        std::string m_lastAttributeName;
        AttributeType m_lastAttributeType;
        CityObjectsTypeMask const m_typeMask; // TODO: Make this a reference once getParserParams doesn't return a copy
        bool m_skipped;

        // The nodes that are valid CityObjects
        static std::mutex initializedTypeIDMutex;
        static std::unordered_map<int, CityObject::CityObjectsType> typeIDTypeMap;
        static bool typeIDTypeMapInitialized;

        static std::mutex initializedAttributeSetMutex;
        static std::unordered_set<int> attributesSet;
        static std::unordered_map<int, AttributeType> attributeTypeMap;
        static bool attributesSetInitialized;

        void parseGeometryForLODLevel(int lod);
        void parseGeometryForLODLevel(int lod, CityObject::CityObjectsType parentType);
        void parseImplicitGeometryForLODLevel(int lod);
        void parseGeometryPropertyElementForLODLevel(int lod, const std::string& id);
    };

}
