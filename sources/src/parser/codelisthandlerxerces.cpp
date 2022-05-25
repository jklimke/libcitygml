#include <parser/codelisthandlerxerces.h>
#include <iostream>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc;

const CodeList CodeListParser::parse(const std::string& filePath) const {
    CodeList code_list;

    try {
        XMLPlatformUtils::Initialize();
    }
    catch (...) {
        throw std::runtime_error("Fail to initialize Xerces-C++.");
    }

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

    parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, false);
    try {
        CodeListHandlerXerces handler;
        parser->setContentHandler(&handler);
        parser->parse(filePath.c_str());
        code_list = handler.getCodeList();
    }
    catch (...) {
        throw std::runtime_error("Fail to analyze xml file.");
    }
    delete parser;
    XMLPlatformUtils::Terminate();

    return code_list;
}

CodeListHandlerXerces::CodeListHandlerXerces() :
    m_definitionTagExists(false),
    m_descriptionTagExists(false),
    m_nameTagExists(false),
    m_description(""),
    m_name("")
{ 
}

CodeList CodeListHandlerXerces::getCodeList() {
    return m_codeList;
}

void CodeListHandlerXerces::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs) {
    char* name = XMLString::transcode(localname);

    if (std::string(name) == "Definition") m_definitionTagExists = true;
    if (std::string(name) == "description" && m_definitionTagExists) m_descriptionTagExists = true;
    if (std::string(name) == "name" && m_definitionTagExists) m_nameTagExists = true;

    XMLString::release(&name);
}

void CodeListHandlerXerces::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname) {
    char* name = XMLString::transcode(localname);

    if (std::string(name) == "description") m_descriptionTagExists = false;
    if (std::string(name) == "name") m_nameTagExists = false;

    if (std::string(name) == "Definition") {
        m_codeList[stoi(m_name)] = m_description;        
        m_definitionTagExists = false;
        m_description = "";
        m_name = "";
    }
    
    XMLString::release(&name);
}

void CodeListHandlerXerces::characters(const XMLCh* const chars, const XMLSize_t length) {
    XMLCh* buffer = new XMLCh[XMLString::stringLen(chars) + 1];
    XMLString::copyString(buffer, chars);
    XMLString::trim(buffer);
    char* content = XMLString::transcode(buffer);
    delete[] buffer;

    if (m_descriptionTagExists) m_description = std::string(content);
    if (m_nameTagExists) m_name = std::string(content);

    XMLString::release(&content);
}