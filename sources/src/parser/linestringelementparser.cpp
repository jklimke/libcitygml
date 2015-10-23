#include "parser/linestringelementparser.h"

#include <unordered_map>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/parserutils.hpp"

#include <citygml/linestring.h>
#include <citygml/citygmlfactory.h>
#include <citygml/citygmllogger.h>

#include <mutex>
#include <stdexcept>

namespace citygml {

    LineStringElementParser::LineStringElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, std::function<void(std::shared_ptr<LineString>)> callback)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
    }

    std::string LineStringElementParser::elementParserName() const
    {
        return "LineStringElementParser";
    }

    bool LineStringElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::GML_LineStringNode || node == NodeType::GML_PointNode;
    }

    bool LineStringElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag <" << NodeType::GML_LineStringNode << "> or <" << NodeType::GML_PointNode << "> but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createLineString(attributes.getCityGMLIDAttribute());

        parseDimension(attributes);

        return true;

    }

    bool LineStringElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        return true;
    }

    bool LineStringElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("LineStringElementParser::parseChildElementStartTag called before LineStringElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_PosListNode
                || node == NodeType::GML_PosNode) {
            parseDimension(attributes);
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool LineStringElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("LineStringElementParser::parseChildElementEndTag called before LineStringElementParser::parseElementStartTag");
        }

        if (node == NodeType::GML_PosListNode
                || node == NodeType::GML_PosNode) {

            if (m_model->getDimensions() < 0) {
                CITYGML_LOG_ERROR(m_logger, "No srsDimension given for LineString before or as attribute of <" << NodeType::GML_PosListNode << "> child element at " << getDocumentLocation());
            } else if (m_model->getDimensions() == 2) {
                m_model->setVertices2D(parseVecList<TVec2d>(characters, m_logger, getDocumentLocation()));
            } else if (m_model->getDimensions() == 3) {
                m_model->setVertices3D(parseVecList<TVec3d>(characters, m_logger, getDocumentLocation()));
            } else {
                CITYGML_LOG_WARN(m_logger, "Unsupported dimension of LineString positions at " << getDocumentLocation() << ". Only 2 and 3 dimensions are supported.");
            }

            return true;
        }

        return GMLObjectElementParser::parseChildElementEndTag(node, characters);

    }

    Object* LineStringElementParser::getObject()
    {
        return m_model.get();
    }

    void LineStringElementParser::parseDimension(Attributes& attributes)
    {
        std::string dim_str = attributes.getAttribute("srsDimension", "");

        if (dim_str.empty()) {
            return;
        }

        int dim = std::stoi(dim_str);
        m_model->setDimensions(dim);
    }

}
