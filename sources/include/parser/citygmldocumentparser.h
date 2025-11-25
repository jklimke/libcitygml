#pragma once

#include <citygml/citygml.h>

#include <stack>
#include <memory>

namespace citygml {

    class Attributes;
    class CityGMLLogger;
    class DocumentLocation;
    class CityGMLFactory;
    class ElementParser;

    class CityGMLDocumentParser {
    public:
        CityGMLDocumentParser(const ParserParams& params, std::shared_ptr<CityGMLLogger> logger, std::unique_ptr<TesselatorBase> tesselator);

        std::shared_ptr<const CityModel> getModel();

        const ParserParams getParserParams() const;

        // Methods used by CityGMLElementParser

        void setCurrentElementParser(ElementParser* parser);
        void removeCurrentElementParser(const ElementParser* caller);

        /**
         * @brief the current location in the document
         */
        virtual const DocumentLocation& getDocumentLocation() const = 0;

        virtual ~CityGMLDocumentParser();

    protected:

        /**
         * @brief must be called for each xml element start tag
         * @param name the name of the xml element
         * @param attributes the attribut data of the xml element
         */
        void startElement( const std::string& name, Attributes& attributes);

        /**
         * @brief must be called for each xml element end tag
         * @param name the name of the xml element
         * @param characters the character data of the element or empty string if it contains no charcter data
         */
        void endElement( const std::string& name, const std::string& characters );

        /**
         * @brief must be called at the start of the document
         */
        void startDocument();

        /**
         * @brief must be called at the end of the document
         */
        void endDocument();


        std::shared_ptr<CityGMLLogger> m_logger;
    private:
        void skipUnknownOrUnexpectedElement(const std::string& name);
        bool checkCurrentElementUnknownOrUnexpected_start(const std::string& name);
        bool checkCurrentElementUnknownOrUnexpected_end(const std::string& name);

        std::stack<std::shared_ptr<ElementParser> > m_parserStack;

        /**
         * @brief The currently active parser (the one on which startElement or endElement was called last)
         *
         * The active parser can remove itself from the stack at any time. Hence we need another shared_ptr reference
         * to it so that it does not delete itself when removed from the stack.
         */
        std::shared_ptr<ElementParser> m_activeParser;

        std::unique_ptr<CityGMLFactory> m_factory;
        std::shared_ptr<CityModel> m_rootModel;
        ParserParams m_parserParams;

        std::unique_ptr<TesselatorBase> m_tesselator;

        bool m_currentElementUnknownOrUnexpected;
        int m_unknownElementOrUnexpectedElementDepth;
        std::string m_unknownElementOrUnexpectedElementName;
    };

}
