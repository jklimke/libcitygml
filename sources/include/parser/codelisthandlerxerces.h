#pragma once

#include <xercesc/sax2/DefaultHandler.hpp>
#include <map>
#include <string>

typedef std::map<int, std::string> CodeList;
typedef std::map<std::string, CodeList> CodeLists;

class CodeListParser {
public:
    const CodeList parse(const std::string& filePath) const;
};

class CodeListHandlerXerces : public xercesc::DefaultHandler {
public:
    CodeListHandlerXerces();
    CodeList getCodeList();
    void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs);
    void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
    void characters(const XMLCh* const chars, const XMLSize_t length);
private:
    bool m_definitionTagExists;
    bool m_descriptionTagExists;
    bool m_nameTagExists;
    std::string m_description;
    std::string m_name;
    CodeList m_codeList;
};