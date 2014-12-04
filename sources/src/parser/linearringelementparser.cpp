#include "parser/linearringelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/parserutils.hpp"

#include "citygml/linearring.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"


namespace citygml {

    LinearRingElementParser::LinearRingElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, bool interior, std::function<void(LinearRing*)> callback)
        : CityGMLElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
        m_interior = interior;
    }

    std::string LinearRingElementParser::elementParserName() const
    {
        return "LinearRingElementParser";
    }

    bool LinearRingElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::GML_LinearRingNode;
    }

    bool LinearRingElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::GML_LinearRingNode << "> but got <" << node << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = new LinearRing(attributes.getCityGMLIDAttribute(), !m_interior);
        return true;

    }

    bool LinearRingElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        if (m_model->getVertices().size() < 4) {
            CITYGML_LOG_WARN(m_logger, "LinearRing with end tag at " << getDocumentLocation() << " contains less than 4 vertices.");
        }
        m_callback(m_model);
        return true;
    }



    bool LinearRingElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("LinearRingElementParser::parseChildElementStartTag called before LinearRingElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_PosListNode || node == NodeType::GML_PosNode) {
            std::string dimensions = attributes.getAttribute("srsDimension", "3");
            if (dimensions != "3") {
                CITYGML_LOG_WARN(m_logger, "Attribute srsDimension of element " << node << " contains unsupported value '" << dimensions << "' (only 3 dimensions are support). Trying to parse it anyway...");
            }
            return true;
        }

        return false;
    }

    bool LinearRingElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("LinearRingElementParser::parseChildElementEndTag called before LinearRingElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_PosListNode) {
            m_model->setVertices(parseVecList<TVec3d>(characters, m_logger, getDocumentLocation()));
            return true;
        } else if (node == NodeType::GML_PosNode) {
            m_model->addVertex(parseValue<TVec3d>(characters, m_logger, getDocumentLocation()));
            return true;
        }

        return false;

    }

}
