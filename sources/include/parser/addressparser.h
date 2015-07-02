#pragma once

#include <parser/citygmlelementparser.h>

namespace citygml {

    class Address;

    class AddressParser: public CityGMLElementParser {
    public:
        using Callback = std::function<void(std::unique_ptr<Address>&&)>;

    public:
        AddressParser(CityGMLDocumentParser& documentParser, CityGMLFactory& factory, std::shared_ptr<CityGMLLogger> logger, const Callback& callback);

        virtual std::string elementParserName() const override;
        virtual bool handlesElement(const NodeType::XMLNode& node) const override;

    protected:
        virtual bool parseElementStartTag(const NodeType::XMLNode& node, Attributes& attributes ) override;
        virtual bool parseElementEndTag(const NodeType::XMLNode& node, const std::string& characters ) override;
        virtual bool parseChildElementStartTag(const NodeType::XMLNode& node, Attributes& attributes ) override;
        virtual bool parseChildElementEndTag(const NodeType::XMLNode& node, const std::string& characters ) override;


    protected:
        std::unique_ptr<Address> m_address;
        Callback m_callback;
    };

} /* namespace citygml */
