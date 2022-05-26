#include <parser/codelisthandlerxerces.h>
#include <iostream>

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc;

namespace {
    std::string toStdString(const XMLCh* const wstr) {
        if (wstr == nullptr) {
            return "";
        }

        char* tmp = XMLString::transcode(wstr);
        std::string str(tmp);
        XMLString::release(&tmp);
        return str;
    }
}

CodeList CodeListParser::parse(const std::string& filePath) const {
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
    m_isParsingDefinition(false),
    m_isParsingDescription(false),
    m_isParsingName(false) {
}

CodeList CodeListHandlerXerces::getCodeList() {
    return m_codeList;
}

void CodeListHandlerXerces::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const xercesc::Attributes& attrs) {
    const auto tagName = toStdString(qname);

    if (tagName == "gml:Definition") {
        m_isParsingDefinition = true;
    } else if (tagName == "gml:description" && m_isParsingDefinition) {
        m_isParsingDescription = true;
    } else if (tagName == "gml:name" && m_isParsingDefinition) {
        m_isParsingName = true;
    }
}

void CodeListHandlerXerces::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname) {
    const auto tagName = toStdString(qname);

    if (tagName == "gml:description") {
        m_isParsingDescription = false;
    } else if (tagName == "gml:name") {
        m_isParsingName = false;
    } else if (tagName == "gml:Definition") {
        m_codeList[m_name] = m_description;
        m_isParsingDefinition = false;
        m_description = "";
        m_name = "";
    }
}

void CodeListHandlerXerces::characters(const XMLCh* const chars, const XMLSize_t length) {
    const auto characters = toStdString(chars);

    if (m_isParsingDescription) {
        m_description = characters;
    } else if (m_isParsingName) {
        m_name = characters;
    }
}
