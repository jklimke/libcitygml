#pragma once

#include <string>
#include <memory>

#include "parser/elementparser.h"
#include "parser/nodetypes.h"

namespace citygml {

    class Attributes;
    class CityGMLDocumentParser;
    class CityGMLLogger;
    class CityGMLFactory;
    class DocumentLocation;

    /**
     * @brief The CityGMLElementParser class is a xml sax parser that is responsible for parsing a specific subset of elements
     *
     * The CityGMLDocumentParser uses CityGMLElementParser objects to parse the elements that are found in the document.
     * Only the CityGMLElementParser objects for the root elements of a citygml document are known by the CityGMLDocumentParser.
     * Whenever it encounters such an element it delegates the startElement and endElement class to the corresponding CityGMLElementParser object.
     * A CityGMLElementParser object can itself invoke other CityGMLElementParser objects to parse its child elements.
     *
     * After a CityGMLElementParser object has been set as the active element parser by the CityGMLDocumentParser or another CityGMLElementParser (the caller)
     * it will be bound to the xml element whose start tag is found next by the sax parser.
     * As soon as the end tag of the bound element is detected the control flow is returned to the caller.
     * How the result values are shared between caller and callee is up to the concrete implementation.
     * Usally a callback mechanism is used.
     */
    class CityGMLElementParser : public ElementParser {
    public:

        CityGMLElementParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger);


        /**
         * @brief must be called for xml tag that starts a child of the elements handeld by this parser or starts one of the elements itself
         * @return true if the node was expected otherwise false
         * @note the CityGMLDocumentParser calls this method
         */
        bool startElement(const NodeType::XMLNode& node, Attributes& attributes) override;

        /**
         * @brief must be called for each xml tag that ends a child of the elements handeld by this parser or ends one of the elements itself
         * @return true if the node was expected otherwise false
         * @note the CityGMLDocumentParser calls this method
         */
        bool endElement(const NodeType::XMLNode& node, const std::string& characters ) override;

        ~CityGMLElementParser() override;

    protected:

        // Template methods to be implemented by subclasses

        /**
         * @brief called for the start tag of the element to which the parser is bound
         * @return true if the node was expected otherwise false
         */
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes ) = 0;

        /**
         * @brief called for the end tag of the element to which the parser is bound
         * @return true if the node was expected otherwise false
         */
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters ) = 0;

        /**
         * @brief called for the start tag of each child inside the element to which the parser is bound
         * @return true if the node was expected otherwise false
         */
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes ) = 0;

        /**
         * @brief called for the end tag of each child inside the element to which the parser is bound
         * @return true if the node was expected otherwise false
         * @note if a callback mechanism is used to share the result of this parser this method would be the right place to invoke the callback
         */
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters ) = 0;

        CityGMLFactory& m_factory;
    private:
        NodeType::XMLNode m_boundElement;
    };

}
