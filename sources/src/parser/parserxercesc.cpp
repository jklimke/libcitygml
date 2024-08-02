/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>

#include <atomic>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>

#include <citygml/citygml_api.h>
#include "citygml/tesselatorbase.h"
#include "parser/citygmldocumentparser.h"
#include "parser/documentlocation.h"
#include "parser/attributes.h"

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax/Locator.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace citygml;

std::string toStdString( const XMLCh* const wstr )
{
    if (wstr == nullptr) {
        return "";
    }

    char* tmp = xercesc::XMLString::transcode(wstr);
    std::string str(tmp);
    xercesc::XMLString::release(&tmp);
    return str;
}

std::shared_ptr<XMLCh> toXercesString(const std::string& str) {

    XMLCh* conv = xercesc::XMLString::transcode(str.c_str());
    // Pack xerces string into shared_ptr with custom delete function
    return std::shared_ptr<XMLCh>(conv, [=](XMLCh* str) {
        xercesc::XMLString::release(&str);
    });
}

class DocumentLocationXercesAdapter : public citygml::DocumentLocation {
public:
    explicit DocumentLocationXercesAdapter(const std::string& fileName)
        : m_locator(nullptr)
        , m_fileName(fileName)
    {

    }

    void setLocator(const xercesc::Locator* locator) {
        m_locator = locator;
    }

    // DocumentLocation interface
    virtual const std::string& getDocumentFileName() const {
        return m_fileName;
    }

    virtual uint64_t getCurrentLine() const {
        return m_locator != nullptr ? m_locator->getLineNumber() : 0;
    }
    virtual uint64_t getCurrentColumn() const {
        return m_locator != nullptr ? m_locator->getColumnNumber() : 0;
    }

protected:
    const xercesc::Locator* m_locator;
    std::string m_fileName;
};

class AttributesXercesAdapter : public citygml::Attributes {
public:
    AttributesXercesAdapter(const xercesc::Attributes& attrs, const citygml::DocumentLocation& docLoc, std::shared_ptr<CityGMLLogger> logger)
     : citygml::Attributes(logger), m_attrs(attrs), m_location(docLoc) {}

    // Attributes interface
    virtual std::string getAttribute(const std::string& attname, const std::string& defvalue) const {
        std::shared_ptr<XMLCh> name = toXercesString(attname);
        std::string value = toStdString(m_attrs.getValue(name.get()));
        return value.empty() ? defvalue : value;
    }

    virtual const DocumentLocation& getDocumentLocation() const {
        return m_location;
    }

protected:
    const xercesc::Attributes& m_attrs;
    const citygml::DocumentLocation& m_location;
};

// CityGML Xerces-c SAX parsing handler
class CityGMLHandlerXerces : public xercesc::DefaultHandler, public citygml::CityGMLDocumentParser
{
public:
    CityGMLHandlerXerces( const ParserParams& params, const std::string& fileName, std::shared_ptr<CityGMLLogger> logger, std::unique_ptr<TesselatorBase> tesselator)
        : citygml::CityGMLDocumentParser(params, logger, std::move(tesselator)), m_documentLocation(DocumentLocationXercesAdapter(fileName)) {}


    // ContentHandler interface
    virtual void startElement(const XMLCh* const, const XMLCh* const, const XMLCh* const qname, const xercesc::Attributes& attrs) override {
        AttributesXercesAdapter attributes(attrs, m_documentLocation, m_logger);
        // We need to empty m_lastcharacters here, because if a tag is empty, characters(...) will never be called and this variable will contain wrong values
        m_lastcharacters = "";
        CityGMLDocumentParser::startElement(toStdString(qname), attributes);
    }

    virtual void endElement(const XMLCh* const, const XMLCh* const, const XMLCh* const qname) override {
        CityGMLDocumentParser::endElement(toStdString(qname), m_lastcharacters);
        m_lastcharacters = "";
    }

    virtual void characters(const XMLCh* const chars, const XMLSize_t) override {
        m_lastcharacters += toStdString(chars);
    }

    virtual void startDocument() override {
        CityGMLDocumentParser::startDocument();
    }

    virtual void endDocument() override {
        CityGMLDocumentParser::endDocument();
    }

    virtual void setDocumentLocator(const xercesc::Locator* const locator) override {
        m_documentLocation.setLocator(locator);
    }

    // CityGMLDocumentParser interface
    virtual const citygml::DocumentLocation& getDocumentLocation() const override {
        return m_documentLocation;
    }
protected:
    DocumentLocationXercesAdapter m_documentLocation;
    std::string m_lastcharacters;

};

class StdBinInputStream : public xercesc::BinInputStream
{
public:
    explicit StdBinInputStream( std::istream& stream ) : BinInputStream(), m_stream( stream ) {}

    virtual ~StdBinInputStream() {}

    virtual XMLFilePos curPos() const { return m_stream.tellg(); }

    virtual XMLSize_t readBytes( XMLByte* const buf, const XMLSize_t maxToRead )
    {
        assert( sizeof(XMLByte) == sizeof(char) );
        if ( !m_stream ) return 0;
        m_stream.read( reinterpret_cast<char*>(buf), maxToRead );
        return (XMLSize_t)m_stream.gcount();
    }

    virtual const XMLCh* getContentType() const { return nullptr; }

private:
    std::istream& m_stream;
};

class StdBinInputSource : public xercesc::InputSource
{
public:
    explicit StdBinInputSource( std::istream& stream ) : m_stream( stream ) {}

    virtual xercesc::BinInputStream* makeStream() const
    {
        return new StdBinInputStream( m_stream );
    }

    ~StdBinInputSource() {
    }

private:
    std::istream& m_stream;
};

// Parsing methods
namespace citygml
{

    class StdLogger : public CityGMLLogger {
    public:

        StdLogger(LOGLEVEL level = LOGLEVEL::LL_ERROR):CityGMLLogger(level){

        };

        virtual void log(LOGLEVEL level, const std::string& message, const char* file, int line) const
        {
            std::ostream& stream = level == LOGLEVEL::LL_ERROR ? std::cerr : std::cout;

            switch(level) {
            case LOGLEVEL::LL_DEBUG:
                stream << "DEBUG";
                break;
            case LOGLEVEL::LL_WARNING:
                stream << "WARNING";
                break;
            case LOGLEVEL::LL_TRACE:
                stream << "TRACE";
                break;
            case LOGLEVEL::LL_ERROR:
                stream << "ERROR";
                break;
            case LOGLEVEL::LL_INFO:
                stream << "INFO";
                break;
            }

            if (file) {
                stream << " [" << file;
                if (line > -1) {
                    stream << ":" << line;
                }
                stream << "]";
            }

            stream << " " << message << std::endl;
        }
    };

    std::mutex xerces_init_mutex;
    std::atomic_bool xerces_initialized;

    bool initXerces(std::shared_ptr<CityGMLLogger> logger) {

        if (xerces_initialized.load()) {
            return true;
        }

        try {
            std::lock_guard<std::mutex> lock(xerces_init_mutex);
            // Check xerces_initialized again... it could have changed while waiting for the mutex
            if (!xerces_initialized.load()) {
                xercesc::XMLPlatformUtils::Initialize();
                xerces_initialized.exchange(true);
            }
        }
        catch (const xercesc::XMLException& e) {
            CITYGML_LOG_ERROR(logger, "Could not initialize xercesc XMLPlatformUtils, a XML Exception occurred : " << toStdString(e.getMessage()));
            return false;
        }

        return true;

    }

    std::shared_ptr<const CityModel> parse(xercesc::InputSource& stream, const ParserParams& params, std::shared_ptr<CityGMLLogger> logger, std::unique_ptr<TesselatorBase> tesselator, std::string filename = "") {



        CityGMLHandlerXerces handler( params, filename, logger, std::move(tesselator) );

        xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();
        parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, false);
        parser->setContentHandler( &handler );
        parser->setErrorHandler( &handler );

#ifdef NDEBUG
        try
        {
#endif
            parser->parse(stream);
#ifdef NDEBUG
        }
        catch ( const xercesc::XMLException& e )
        {
            CITYGML_LOG_ERROR(logger, "XML Exception occurred: " << toStdString(e.getMessage()));
        }
        catch ( const xercesc::SAXParseException& e )
        {
            CITYGML_LOG_ERROR(logger, "SAXParser Exception occurred: " << toStdString(e.getMessage()));
        }
        catch ( const std::exception& e )
        {
            CITYGML_LOG_ERROR(logger, "Unexpected Exception occurred: " << e.what());
        }
#endif

            delete parser;

        return handler.getModel();
    }

    std::shared_ptr<const CityModel> load(std::istream& stream, const ParserParams& params, std::unique_ptr<TesselatorBase> tesselator, std::shared_ptr<CityGMLLogger> logger)
    {
        if (!logger) {
            logger = std::make_shared<StdLogger>();
            if(tesselator) {
                tesselator->setLogger(logger);
            }
        }

        if (!initXerces(logger)) {
            return nullptr;
        }

        StdBinInputSource streamSource(stream);
        return parse(streamSource, params, logger, std::move(tesselator));
    }

    std::shared_ptr<const CityModel> load( const std::string& fname, const ParserParams& params, std::unique_ptr<TesselatorBase> tesselator, std::shared_ptr<CityGMLLogger> logger)
    {
        if (!logger) {
            logger = std::make_shared<StdLogger>();
            if(tesselator) {
                tesselator->setLogger(logger);
            }
        }

        if (!initXerces(logger)) {
            return nullptr;
        }

        std::shared_ptr<XMLCh> fileName = toXercesString(fname);

#ifdef NDEBUG
        try {
#endif
            xercesc::LocalFileInputSource fileSource(fileName.get());
            return parse(fileSource, params, logger, std::move(tesselator), fname);
#ifdef NDEBUG
        } catch (xercesc::XMLException& e) {
            CITYGML_LOG_ERROR(logger, "Error parsing file " << fname << ": " << e.getMessage());
            return nullptr;
        }
#endif

    }
}

