#include "parser/gmlfeaturecollectionparser.h"

#include "parser/attributes.h"
#include "parser/parserutils.hpp"
#include "parser/nodetypes.h"

#include <citygml/citygmllogger.h>
#include <citygml/envelope.h>
#include <citygml/object.h>
#include <citygml/vecs.hpp>
#include <citygml/featureobject.h>


#include <stdexcept>

namespace citygml {

    GMLFeatureCollectionElementParser::GMLFeatureCollectionElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger)
        : GMLObjectElementParser(documentParser, factory, logger)
    {
        m_bounds = nullptr;
    }

    bool GMLFeatureCollectionElementParser::parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes)
    {
        if (getFeatureObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLFeatureCollectionElementParser::parseChildElementStartTag");
        }

        if (node == NodeType::GML_LowerCornerNode
                || node == NodeType::GML_UpperCornerNode
                || node == NodeType::GML_BoundedByNode) {
            return true;
        } else if (node == NodeType::GML_EnvelopeNode) {

            if (m_bounds != nullptr) {
                CITYGML_LOG_WARN(m_logger, "Duplicate definition of " << NodeType::GML_EnvelopeNode << " at " << getDocumentLocation());
                return true;
            }
            m_bounds = new Envelope(attributes.getAttribute("srsName"));
            return true;
        }

        return GMLObjectElementParser::parseChildElementStartTag(node, attributes);
    }

    bool GMLFeatureCollectionElementParser::parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters)
    {
        if (getFeatureObject() == nullptr) {
            throw std::runtime_error("Invalid call to GMLFeatureCollectionElementParser::parseChildElementEndTag");
        }

        if (node == NodeType::GML_LowerCornerNode) {

            if (m_bounds != nullptr) {
                m_bounds->setLowerBound(parseValue<TVec3d>(characters, m_logger, getDocumentLocation()));
            } else {
                CITYGML_LOG_WARN(m_logger, "Definition of " << NodeType::GML_LowerCornerNode << " outside " << NodeType::GML_EnvelopeNode << " at " << getDocumentLocation());
            }
            return true;
        } else if (node == NodeType::GML_UpperCornerNode) {

            if (m_bounds != nullptr) {
                m_bounds->setUpperBound(parseValue<TVec3d>(characters, m_logger, getDocumentLocation()));
            } else {
                CITYGML_LOG_WARN(m_logger, "Definition of " << NodeType::GML_UpperCornerNode << " outside " << NodeType::GML_EnvelopeNode << " at " << getDocumentLocation());
            }
            return true;
        } else if (node == NodeType::GML_EnvelopeNode) {

            getFeatureObject()->setEnvelope(m_bounds);
            return true;
        } else if (node == NodeType::GML_BoundedByNode) {

            return true;
        }

        return GMLObjectElementParser::parseChildElementEndTag(node, characters);
    }

    Object* GMLFeatureCollectionElementParser::getObject()
    {
        return getFeatureObject();
    }



}
