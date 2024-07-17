#include "parser/citygmldocumentparser.h"
#include "parser/delayedchoiceelementparser.h"
#include "parser/documentlocation.h"
#include "parser/nodetypes.h"
#include "parser/elementparser.h"
#include "parser/citymodelelementparser.h"
#include "parser/geocoordinatetransformer.h"

#include <citygml/citygmllogger.h>
#include <citygml/citygmlfactory.h>
#include <citygml/citymodel.h>
#include <citygml/tesselatorbase.h>

#include <stdexcept>

namespace citygml {

    CityGMLDocumentParser::CityGMLDocumentParser(const ParserParams& params, std::shared_ptr<CityGMLLogger> logger, std::unique_ptr<TesselatorBase> tesselator)
    {
        m_logger = logger;
        m_factory = std::unique_ptr<CityGMLFactory>(new CityGMLFactory(logger));
        m_parserParams = params;
        m_tesselator = std::move(tesselator);
        m_activeParser = nullptr;
        m_currentElementUnknownOrUnexpected = false;
        m_unknownElementOrUnexpectedElementDepth = 0;
        m_unknownElementOrUnexpectedElementName = "";
    }

    std::shared_ptr<const CityModel> CityGMLDocumentParser::getModel()
    {
        return m_rootModel;
    }

    const ParserParams CityGMLDocumentParser::getParserParams() const
    {
        return m_parserParams;
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
        if (checkCurrentElementUnknownOrUnexpected_start(name)) {
            CITYGML_LOG_DEBUG(m_logger, "Skipping element <" << name << "> at " << getDocumentLocation());
            return;
        }

        const NodeType::XMLNode& node = NodeType::getXMLNodeFor(name);

        if (!node.valid()) {
            CITYGML_LOG_WARN(m_logger, "Found start tag of unknown node <" << name << "> at " << getDocumentLocation() << ". Skip to next element.");
            skipUnknownOrUnexpectedElement(name);
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
            CITYGML_LOG_WARN(m_logger, "Skipping element with unexpected start tag <" << node << "> at " << getDocumentLocation() << " (active parser " << m_activeParser->elementParserName() << ")");
            skipUnknownOrUnexpectedElement(name);
        }
    }

    void CityGMLDocumentParser::endElement(const std::string& name, const std::string& characters)
    {
        if (checkCurrentElementUnknownOrUnexpected_end(name)) {
            CITYGML_LOG_DEBUG(m_logger, "Skipped element <" << name << "> at " << getDocumentLocation());
            return;
        }

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
        
        if(std::dynamic_pointer_cast<DelayedChoiceElementParser>(m_activeParser) != nullptr) {
            // We try to close a DelayedChoiceElementParser:
            // this means no node has been found that contains any of the delayed choice nodes
            // This is not directly an error: this needs to be treated as an empty node
            CITYGML_LOG_DEBUG(m_logger, "End tag found while the active parser is a DelayedChoiceElementParser. This probably means the node is empty. The close node is: <" << node << "> at " << getDocumentLocation());
            removeCurrentElementParser(m_activeParser.get());
            m_activeParser = m_parserStack.top();
        }
        if (!m_activeParser->endElement(node, characters)) {
            CITYGML_LOG_ERROR(m_logger, "Active parser " << m_activeParser->elementParserName() << " reports end tag <" << node << "> at " << getDocumentLocation() << " as "
                              << "unknown, but it seems as if the corresponding start tag was not reported as unknown. Please check the parser implementation."
                              << "Ignoring end tag and continue parsing.");
        }

    }

    void CityGMLDocumentParser::startDocument()
    {
        CITYGML_LOG_INFO(m_logger, "Start parsing citygml file (" << getDocumentLocation() << ")");
    }

    void CityGMLDocumentParser::endDocument()
    {
        if (!m_parserStack.empty()) {
            CITYGML_LOG_WARN(m_logger, "Reached end of document but the parser stack is not empty (either a bug or corrupted xml document)");
        }

        CITYGML_LOG_INFO(m_logger, "Finished parsing ciytgml file (" << getDocumentLocation() << ")");

        m_factory->closeFactory();

        if (m_rootModel != nullptr) {

            if(m_tesselator != nullptr) {
                m_tesselator->setKeepVertices(m_parserParams.keepVertices);

                CITYGML_LOG_INFO(m_logger, "Start postprocessing of the citymodel.");
                m_rootModel->finish(m_tesselator.get(), m_parserParams.optimize, m_parserParams.tesselate, m_logger);
                CITYGML_LOG_INFO(m_logger, "Finished postprocessing of the citymodel.");
            }

            m_rootModel->setThemes(m_factory->getAllThemes());

            if (!m_parserParams.destSRS.empty()) {
                try {
                    CITYGML_LOG_INFO(m_logger, "Start coordinates transformation .");
                    GeoCoordinateTransformer transformer(m_parserParams.destSRS, m_logger);
                    transformer.transformToDestinationSRS(m_rootModel.get());
                    CITYGML_LOG_INFO(m_logger, "Finished coordinates transformation .");
                } catch (const std::runtime_error& e) {
                    CITYGML_LOG_ERROR(m_logger, "Coordinate transformation aborted: " << e.what());
                    throw e;
                }
            }

        } else {
            CITYGML_LOG_WARN(m_logger, "Reached end of document but no CityModel was parsed.");
        }
    }

    void CityGMLDocumentParser::skipUnknownOrUnexpectedElement(const std::string& name)
    {
        m_unknownElementOrUnexpectedElementName = name;
        m_unknownElementOrUnexpectedElementDepth = 0;
        m_currentElementUnknownOrUnexpected = true;
    }

    bool CityGMLDocumentParser::checkCurrentElementUnknownOrUnexpected_start(const std::string& name)
    {
        if (!m_currentElementUnknownOrUnexpected) {
            return false;
        }

        if (m_unknownElementOrUnexpectedElementName == name) {
            m_unknownElementOrUnexpectedElementDepth++;
        }

        return true;
    }

    bool CityGMLDocumentParser::checkCurrentElementUnknownOrUnexpected_end(const std::string& name)
    {
        if (!m_currentElementUnknownOrUnexpected) {
            return false;
        }

        if (m_unknownElementOrUnexpectedElementName == name) {
            if (m_unknownElementOrUnexpectedElementDepth == 0) {
                // End tag of initial unknown element reached...
                m_unknownElementOrUnexpectedElementName = "";
                m_currentElementUnknownOrUnexpected = false;
            }
            m_unknownElementOrUnexpectedElementDepth--;
        }

        return true;
    }

    CityGMLDocumentParser::~CityGMLDocumentParser()
    {

    }

}
