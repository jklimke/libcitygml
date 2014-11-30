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

// This is the implementation file for Xerces-c parser

#ifdef USE_XERCESC

#include <fstream>

#include "citygml/citygml.h"
#include "citygml/citygmllogger.h"

#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>

using namespace citygml;

std::string wstos( const XMLCh* const wstr )
    {
#ifdef MSVC
        std::wstring w( (const wchar_t*)wstr );
        return std::string( w.begin(), w.end() );
#else
        char* tmp = xercesc::XMLString::transcode(wstr);
        std::string str(tmp);
        xercesc::XMLString::release(&tmp);
        return str;
#endif
}

// CityGML Xerces-c SAX parsing handler
class CityGMLHandlerXerces : public xercesc::HandlerBase
{
public:
    CityGMLHandlerXerces( const ParserParams& params, std::shared_ptr<CityGMLLogger> logger) {}

protected:
    void startElement( const XMLCh* const name, xercesc::AttributeList& attr )
    {
//        CityGMLHandler::startElement( wstos( name ), &attr );
//        m_characterData = "";
    }

    void endElement( const XMLCh* const name )
    {
//        CityGMLHandler::endElement( wstos( name ), m_characterData );
    }

    void characters( const XMLCh* const chars, const XMLSize_t length )
    {
        m_characterData = std::string((const char*) chars, length);
    }

    void fatalError( const xercesc::SAXParseException& e )
    {
//        CITYGML_LOG_ERROR(_logger, "Fatal..." << wstos( e.getMessage() ) );
    }

    std::string getAttribute( void* attributes, const std::string& attname, const std::string& defvalue = "" )
    {
        if (!attributes) return defvalue;
        xercesc::AttributeList* attrs = (xercesc::AttributeList*)attributes;
        const XMLCh* att = attrs->getValue( attname.c_str() );
        return att ? wstos( att ) : defvalue;
    }

    std::string m_characterData;
};

class StdBinInputStream : public xercesc::BinInputStream
{
public:
    StdBinInputStream( std::istream& stream ) : BinInputStream(), m_stream( stream ) {}

    virtual ~StdBinInputStream() {}

    virtual XMLFilePos curPos() const { return m_stream.tellg(); }

    virtual XMLSize_t readBytes( XMLByte* const buf, const XMLSize_t maxToRead )
    {
        assert( sizeof(XMLByte) == sizeof(char) );
        if ( !m_stream ) return 0;
        m_stream.read( reinterpret_cast<char*>(buf), maxToRead );
        return (XMLSize_t)m_stream.gcount();
    }

    virtual const XMLCh* getContentType() const { return 0; }

private:
    std::istream& m_stream;
};

class StdBinInputSource : public xercesc::InputSource
{
public:
    StdBinInputSource( std::istream& stream ) : m_stream( stream ) {}

    virtual xercesc::BinInputStream* makeStream() const
    {
        return new StdBinInputStream( m_stream );
    }

private:
    std::istream& m_stream;
};

// Parsing methods
namespace citygml
{

    class StdLogger : public CityGMLLogger {
    public:
        virtual void log(LOGLEVEL level, const std::string& message, const char* file, int line) const
        {
            std::ostream& stream = level == LOGLEVEL::ERROR ? std::cerr : std::cout;

            switch(level) {
            case LOGLEVEL::DEBUG:
                stream << "DEBUG";
                break;
            case LOGLEVEL::WARNING:
                stream << "WARNING";
                break;
            case LOGLEVEL::TRACE:
                stream << "TRACE";
                break;
            case LOGLEVEL::ERROR:
                stream << "ERROR";
                break;
            case LOGLEVEL::INFO:
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

        virtual bool isEnabledFor(LOGLEVEL) const
        {
            return true;
        }
    };

    CityModel* load(std::istream& stream, const ParserParams& params, std::shared_ptr<CityGMLLogger> logger)
    {

        if (!logger) {
            logger = std::make_shared<StdLogger>();
        }

        try
        {
            xercesc::XMLPlatformUtils::Initialize();
        }
        catch ( const xercesc::XMLException& e )
        {
//            CITYGML_LOG_ERROR(logger, "XML Exception occures during initialization!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ));
            return nullptr;
        }

        CityGMLHandlerXerces* handler = new CityGMLHandlerXerces( params, logger );

        xercesc::SAXParser* parser = new xercesc::SAXParser();
        parser->setDoNamespaces( false );
        parser->setDocumentHandler( handler );
        parser->setErrorHandler( handler );

        CityModel* model = 0;

//        try
//        {
//            StdBinInputSource input( stream );
//            parser->parse( input );
//            model = handler->getModel();
//        }
//        catch ( const xercesc::XMLException& e )
//        {
//            CITYGML_LOG_ERROR(logger, "XML Exception occured!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ));
//            delete handler->getModel();
//        }
//        catch ( const xercesc::SAXParseException& e )
//        {
//            CITYGML_LOG_ERROR(logger, "SAXParser Exception occured!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ));
//            delete handler->getModel();
//        }
//        catch ( ... )
//        {
//            CITYGML_LOG_ERROR(logger, "Unexpected Exception occured!");
//            delete handler->getModel();
//        }

        delete parser;
        delete handler;
        return model;
    }

    CityModel* load( const std::string& fname, const ParserParams& params , std::shared_ptr<CityGMLLogger> logger)
    {
        if (!logger) {
            logger = std::make_shared<StdLogger>();
        }

        std::ifstream file;
        file.open( fname.c_str(), std::ifstream::in );
        if ( file.fail() ) { CITYGML_LOG_ERROR(logger, "CityGML: Unable to open file " << fname << "!"); return 0; }
        CityModel* model = load( file, params, logger );
        file.close();
        return model;
    }
}

#endif
