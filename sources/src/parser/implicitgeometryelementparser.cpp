#include "parser/implicitgeometryelementparser.h"

#include <unordered_set>

#include "parser/nodetypes.h"
#include "parser/attributes.h"
#include "parser/documentlocation.h"
#include "parser/geometryelementparser.h"
#include "parser/parserutils.hpp"
#include "parser/skipelementparser.h"

#include "citygml/implictgeometry.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citygmllogger.h"

#include <stdexcept>

namespace citygml {


    ImplicitGeometryElementParser::ImplicitGeometryElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger,
                                                 int lodLevel, CityObject::CityObjectsType parentType,  std::function<void(ImplicitGeometry*)> callback)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_callback = callback;
        m_lodLevel = lodLevel;
        m_parentType = parentType;
    }

    std::string ImplicitGeometryElementParser::elementParserName() const
    {
        return "ImplicitGeometryElementParser";
    }

    bool ImplicitGeometryElementParser::handlesElement(const NodeType::XMLNode& node) const
    {
        return node == NodeType::CORE_ImplicitGeometryNode;
    }

    bool ImplicitGeometryElementParser::parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {

        if (!handlesElement(node)) {
            CITYGML_LOG_ERROR(m_logger, "Expected start tag of ImplicitGeometryObject but got <" << node.name() << "> at " << getDocumentLocation());
            throw std::runtime_error("Unexpected start tag found.");
        }

        m_model = m_factory.createImplictGeometry(attributes.getCityGMLIDAttribute());
        return true;

    }

    bool ImplicitGeometryElementParser::parseElementEndTag(const NodeType::XMLNode&, const std::string&)
    {
        m_callback(m_model);
        return true;
    }



    bool ImplicitGeometryElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (m_model == nullptr) {
            throw std::runtime_error("ImplicitGeometryElementParser::parseChildElementStartTag called before ImplicitGeometryElementParser::parseElementStartTag");
        }

        if (   node == NodeType::CORE_TransformationMatrixNode
            || node == NodeType::CORE_ReferencePointNode
            || node == NodeType::GML_ReferencePointNode
            || node == NodeType::CORE_MimeTypeNode) {

            return true;

        } else if (node == NodeType::GML_PointNode) {

            m_model->setSRSName(attributes.getAttribute("srsName"));
            return true;
        } else if (node == NodeType::GML_PosNode) {

            std::string srsDimension = attributes.getAttribute("srsDimension","3");
            if (srsDimension != "3") {
                CITYGML_LOG_WARN(m_logger, NodeType::GML_PosNode << " element at " << getDocumentLocation() << " in ImplicitGeometry node has an unsupported 'srsDimension' attribute value of " << srsDimension
                                 << " (Only 3 is supported). Trying to parse it anyway.");
            }
            return true;
        } else if (node == NodeType::CORE_RelativeGMLGeometryNode) {

            if (attributes.hasXLinkAttribute()) {

                std::string sharedGeomID = attributes.getXLinkValue();
                m_factory.requestSharedGeometryWithID(m_model, sharedGeomID);
            } else {

                setParserForNextElement(new GeometryElementParser(m_documentParser, m_factory, m_logger, m_lodLevel, m_parentType, [this](Geometry* geom) {
                                        std::shared_ptr<Geometry> shared = m_factory.shareGeometry(geom);
                                        m_model->addGeometry(shared);
                                    }));
            }
            return true;
        } else if (node == NodeType::CORE_LibraryObjectNode) {
            CITYGML_LOG_INFO(m_logger, "Skipping ImplicitGeometry child element <" << node  << ">  at " << getDocumentLocation() << " (Currently not supported!)");
            setParserForNextElement(new SkipElementParser(m_documentParser, m_logger));
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool ImplicitGeometryElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {

        if (m_model == nullptr) {
            throw std::runtime_error("ImplicitGeometryElementParser::parseChildElementEndTag called before ImplicitGeometryElementParser::parseElementStartTag");
        }

        if (node == NodeType::CORE_TransformationMatrixNode) {

            m_model->setTransformMatrix(parseMatrix(characters, m_logger, getDocumentLocation()));
            return true;

        } else if (node == NodeType::GML_PosNode) {

            m_model->setReferencePoint(parseValue<TVec3d>(characters, m_logger, getDocumentLocation()));
            return true;
        } else if (   node == NodeType::CORE_RelativeGMLGeometryNode
                   || node == NodeType::GML_PointNode
                   || node == NodeType::CORE_ReferencePointNode
                   || node == NodeType::GML_ReferencePointNode
                   || node == NodeType::CORE_LibraryObjectNode) {

            return true;
        } else if (node == NodeType::CORE_MimeTypeNode) {
            m_model->setAttribute(node.name(), characters);
        }

        return GMLObjectElementParser::parseChildElementEndTag(node, characters);

    }

    Object* ImplicitGeometryElementParser::getObject()
    {
        return m_model;
    }

}
