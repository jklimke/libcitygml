#include <parser/addressparser.h>
#include <parser/documentlocation.h>
#include <parser/attributes.h>

#include <citygml/address.h>
#include <citygml/citygmllogger.h>

#include <unordered_set>
#include <unordered_map>
#include <functional>

namespace citygml {

    namespace {

        template<typename T, typename... Args>
        std::unique_ptr<T> make_unique(Args&&... args) {
            return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

        auto k_rootElements = std::unordered_set<NodeType::XMLNode>();
        auto k_subElements = std::unordered_set<NodeType::XMLNode>();
        auto k_dataElements = std::unordered_map<NodeType::XMLNode, std::function<void(Address*, const std::string&)>>();

        std::mutex g_nodeSetMutex;
        bool g_nodeSetsInitialized = false;

        void initializeNodeSets() {
            if (!g_nodeSetsInitialized) {
                std::lock_guard<std::mutex> lock(g_nodeSetMutex);
                if (!g_nodeSetsInitialized) {
                    k_rootElements = {
                        NodeType::CORE_AddressNode,
                        NodeType::BLDG_AddressNode,
                        NodeType::CORE_XalAddressNode
                    };

                    k_subElements = {
                        NodeType::XAL_AddressDetailsNode,
                        NodeType::XAL_CountryNode,
                        NodeType::XAL_LocalityNode,
                        NodeType::XAL_PostalCodeNode,
                        NodeType::XAL_ThoroughfareNode
                    };

                    k_dataElements = {
                        { NodeType::XAL_CountryNameNode, &Address::setCountry },
                        { NodeType::XAL_LocalityNameNode, &Address::setLocality },
                        { NodeType::XAL_ThoroughfareNameNode, &Address::setThoroughfareName },
                        { NodeType::XAL_ThoroughfareNumberNode, &Address::setThoroughfareNumber },
                        { NodeType::XAL_PostalCodeNumberNode, &Address::setPostalCode }
                    };

                    g_nodeSetsInitialized = true;
                }
            }
        }

    }  // anonymous namespace

    AddressParser::AddressParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, const Callback& callback)
    : CityGMLElementParser(documentParser, factory, logger)
    , m_callback(callback)
    {
        initializeNodeSets();
    }

    std::string AddressParser::elementParserName() const
    {
        return "AddressParser";
    }

    bool AddressParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return k_rootElements.count(node) > 0;
    }

    bool AddressParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (k_rootElements.count(node) == 0) {
            CITYGML_LOG_ERROR(m_logger, "Expected an address start tag but got <" << node << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_address = make_unique<Address>(attributes.getCityGMLIDAttribute());
        return true;
    }

    bool AddressParser::parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        m_callback(std::move(m_address));
        m_address = nullptr;
        return k_rootElements.count(node) > 0;
    }

    bool AddressParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        return k_subElements.count(node) > 0 || k_dataElements.count(node) > 0 || k_rootElements.count(node) > 0;
    }

    bool AddressParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        const auto itr = k_dataElements.find(node);
        if (itr != k_dataElements.end()) {
            itr->second(m_address.get(), characters);
            return true;
        }

        return k_subElements.count(node) > 0 || k_dataElements.count(node) > 0 || k_rootElements.count(node) > 0;
    }

} /* namespace citygml */
