#pragma once

#include <string>
#include <memory>

#include "parser/nodetypes.h"

namespace citygml {

    class Attributes;
    class CityGMLDocumentParser;
    class CityGMLLogger;
    class CityGMLFactory;
    class DocumentLocation;

    /**
     * @brief The ElementParser is the base class for parsers that only handle a specific subset of elements
     *
     * For a basic explanation of the parsing process @see CityGMLElementParser
     */
    class ElementParser {
    public:

        ElementParser(CityGMLDocumentParser& documentParser, std::shared_ptr<CityGMLLogger> logger) : m_logger(logger), m_documentParser(documentParser) {}

        /**
         * @brief must be called for xml tag that starts a child of the elements handeld by this parser or starts one of the elements itself
         * @return true if the node was expected otherwise false
         * @note the CityGMLDocumentParser calls this method
         */
        virtual bool startElement(const NodeType::XMLNode& node, Attributes& attributes) = 0;

        /**
         * @brief must be called for each xml tag that ends a child of the elements handeld by this parser or ends one of the elements itself
         * @return true if the node was expected otherwise false
         * @note the CityGMLDocumentParser calls this method
         */
        virtual bool endElement(const NodeType::XMLNode& node, const std::string& characters ) = 0;

        /**
         * @brief returns wether the parser handels elements of type node
         * @note this is required for the delayed choice mechanism @see DelayedChoiceElementParser
         */
        virtual bool handlesElement(const NodeType::XMLNode& node) const = 0;

        /**
         * @brief the name of the parser (for logging purposes)
         */
        virtual std::string elementParserName() const = 0;

        virtual ~ElementParser();

    protected:
        /**
         * @brief sets a parser that will be called for the next element.
         * @note The parser will be deleted after parsing the elment.
         */
        void setParserForNextElement(ElementParser* parser);
        virtual const DocumentLocation& getDocumentLocation() const;

        std::shared_ptr<CityGMLLogger> m_logger;
        CityGMLDocumentParser& m_documentParser;
    };

}
