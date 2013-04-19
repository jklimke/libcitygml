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

#include "parser.h"

#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/InputSource.hpp>
#include <xercesc/util/BinInputStream.hpp>

using namespace citygml;

// CityGML Xerces-c SAX parsing handler
class CityGMLHandlerXerces : public CityGMLHandler, public xercesc::HandlerBase 
{
public:
	CityGMLHandlerXerces( const ParserParams& params ) : CityGMLHandler( params ) {}

	void startElement( const XMLCh* const name, xercesc::AttributeList& attr )
	{
		CityGMLHandler::startElement( wstos( name ), &attr );
	}

	void endElement( const XMLCh* const name ) 
	{
		CityGMLHandler::endElement( wstos( name ) );
	}

	void characters( const XMLCh* const chars, const XMLSize_t length )
	{
		for ( unsigned int i = 0; i < length; i++ ) _buff << (char)chars[i]; 
	}

	void fatalError( const xercesc::SAXParseException& e ) 
	{
		CityGMLHandler::fatalError( wstos( e.getMessage() ) );
	}

	static inline std::string wstos( const XMLCh* const wstr ) 
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

protected:
	std::string getAttribute( void* attributes, const std::string& attname, const std::string& defvalue = "" )
	{
		if (!attributes) return defvalue;
		xercesc::AttributeList* attrs = (xercesc::AttributeList*)attributes;
		const XMLCh* att = attrs->getValue( attname.c_str() );
		return att ? wstos( att ) : defvalue;
	}
};

class StdBinInputStream : public xercesc::BinInputStream
{
public:
	StdBinInputStream( std::istream& stream ) : BinInputStream(), m_stream( stream ) {}

	virtual ~StdBinInputStream( void ) {}

	virtual XMLFilePos curPos( void ) const { return m_stream.tellg(); }

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
	CityModel* load( std::istream& stream, const ParserParams& params )
	{
		try 
		{
			xercesc::XMLPlatformUtils::Initialize();
		}
		catch ( const xercesc::XMLException& e ) 
		{
			std::cerr << "CityGML: XML Exception occures during initialization!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ) << std::endl;
			return false;
		}

		CityGMLHandlerXerces* handler = new CityGMLHandlerXerces( params );

		xercesc::SAXParser* parser = new xercesc::SAXParser();
		parser->setDoNamespaces( false );
		parser->setDocumentHandler( handler );
		parser->setErrorHandler( handler );

		CityModel* model = 0;

		try 
		{
			StdBinInputSource input( stream );
			parser->parse( input );
			model = handler->getModel();
		}
		catch ( const xercesc::XMLException& e ) 
		{
			std::cerr << "CityGML: XML Exception occures!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ) << std::endl;
			delete handler->getModel();
		}
		catch ( const xercesc::SAXParseException& e ) 
		{
			std::cerr << "CityGML: SAXParser Exception occures!" << std::endl << CityGMLHandlerXerces::wstos( e.getMessage() ) << std::endl;
			delete handler->getModel();
		}
		catch ( ... ) 
		{
			std::cerr << "CityGML: Unexpected Exception occures!" << std::endl ;
			delete handler->getModel();
		}

		delete parser;
		delete handler;
		return model;
	}

	CityModel* load( const std::string& fname, const ParserParams& params )
	{
		std::ifstream file;
		file.open( fname.c_str(), std::ifstream::in );
		if ( file.fail() ) { std::cerr << "CityGML: Unable to open file " << fname << "!" << std::endl; return 0; }
		CityModel* model = load( file, params );
		file.close();
		return model;
	}
}

#endif
