#include "parser/citygmldocumentparser.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"
#include "parser/elementparser.h"
#include "parser/citymodelelementparser.h"
#include "parser/geocoordinatetransformer.h"

#include "citygml/citygmllogger.h"
#include "citygml/citygmlfactory.h"
#include "citygml/citymodel.h"
#include "citygml/tesselator.h"

namespace citygml {

    CityGMLDocumentParser::CityGMLDocumentParser(const ParserParams& params, std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
        m_factory = std::unique_ptr<CityGMLFactory>(new CityGMLFactory(logger));
        m_parserParams = params;
        m_activeParser = nullptr;
    }

    std::shared_ptr<const CityModel> CityGMLDocumentParser::getModel()
    {
        return m_rootModel;
    }

    void CityGMLDocumentParser::setCurrentElementParser(ElementParser* parser)
    {
        m_parserStack.push(std::shared_ptr<ElementParser>(parser));
    }

    void CityGMLDocumentParser::removeCurrentElementParser(const ElementParser* caller)
    {
        if (m_parserStack.top().get() != caller) {
            throw std::runtime_error("A CityGMLElementParser object tries to remove another CityGMLElementParser object from the control flow which is not allowed.");
        }
        m_parserStack.pop();
    }

    void CityGMLDocumentParser::startElement(const std::string& name, Attributes& attributes)
    {

        const NodeType::XMLNode& node = NodeType::getXMLNodeFor(name);

        if (!node.valid()) {
            CITYGML_LOG_WARN(m_logger, "Found start tag of unknown node <" << name << "> at " << getDocumentLocation() << ". Skip to child or next element.");
            return;
        }

        if (m_parserStack.empty()) {
            m_parserStack.push(std::unique_ptr<CityModelElementParser>(new CityModelElementParser(*this, *m_factory, m_logger, [this](CityModel* cityModel) {
                this->m_rootModel = std::unique_ptr<CityModel>(cityModel);
            })));
        }

        m_activeParser = m_parserStack.top();
        CITYGML_LOG_TRACE(m_logger, "Invoke " << m_activeParser->elementParserName() << "::startElement for <" << node << "> at " << getDocumentLocation());
        if (!m_activeParser->startElement(node, attributes)) {
            CITYGML_LOG_INFO(m_logger, "Ignoring unexpected start tag <" << node << "> at " << getDocumentLocation() << " (active parser " << m_activeParser->elementParserName() << ")");
        }
    }

    void CityGMLDocumentParser::endElement(const std::string& name, const std::string& characters)
    {
        const NodeType::XMLNode& node = NodeType::getXMLNodeFor(name);

        if (!node.valid()) {
            CITYGML_LOG_WARN(m_logger, "Found end tag of unknown node <" << name << "> at " << getDocumentLocation());
            return;
        }

        if (m_parserStack.empty()) {
            CITYGML_LOG_ERROR(m_logger, "Found element end tag at" << getDocumentLocation() << "but parser stack is empty (either a bug or corrupted xml document)");
            throw std::runtime_error("Unexpected element end.");
        }

        m_activeParser = m_parserStack.top();
        CITYGML_LOG_TRACE(m_logger, "Invoke " << m_activeParser->elementParserName() << "::endElement for <" << node << "> at " << getDocumentLocation());
        if (!m_activeParser->endElement(node, characters)) {
            CITYGML_LOG_INFO(m_logger, "Ignoring unexpected end tag <" << node << "> at " << getDocumentLocation() << " (active parser " << m_activeParser->elementParserName() << ")");
        }

    }

    void CityGMLDocumentParser::endDocument()
    {
        if (!m_parserStack.empty()) {
            CITYGML_LOG_WARN(m_logger, "Reached end of document but the parser stack is not empty (either a bug or corrupted xml document)");
        }

        m_factory->closeFactory();

        if (m_rootModel != nullptr) {
            Tesselator tesselator(m_logger);
            m_rootModel->finish(m_parserParams.tesselate, tesselator);
            m_rootModel->setThemes(m_factory->getAllThemes());

            if (!m_parserParams.destSRS.empty()) {
                try {
                    GeoCoordinateTransformer transformer(m_parserParams.destSRS, m_logger);
                    transformer.transformToDestinationSRS(m_rootModel.get());
                } catch (const std::runtime_error& e) {
                    CITYGML_LOG_ERROR(m_logger, "Coordinate transformation aborted: " << e.what());
                }
            }

        } else {
            CITYGML_LOG_WARN(m_logger, "Reached end of document but no CityModel was parsed.");
        }
    }

    CityGMLDocumentParser::~CityGMLDocumentParser()
    {

    }

}
