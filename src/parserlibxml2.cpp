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

// This is the implementation file for LibXml2 parser

#ifdef USE_LIBXML2

#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>

using namespace citygml;

// CityGML LibXml2 SAX parsing handler

class CityGMLHandlerLibXml2 : public CityGMLHandler
{
public:
	CityGMLHandlerLibXml2( const ParserParams& params ) : CityGMLHandler( params ) {}

	void startElement( const xmlChar* name, const xmlChar** attrs ) 
	{
		CityGMLHandler::startElement( wstos( name ), attrs );
	}

	void endElement( const xmlChar* name )
	{
		CityGMLHandler::endElement( wstos( name ) );
	}

	void characters( const xmlChar *chars, int length ) 
	{
		for ( int i = 0; i < length; i++ ) _buff << (char)chars[i]; 
	}

	static inline std::string wstos( const xmlChar* const str ) 
	{
		return std::string( (const char*)str );
	}

protected:
	std::string getAttribute( void* attributes, const std::string& attname, const std::string& defvalue = "" )
	{
		const xmlChar **attrs = (const xmlChar**)attributes;
		if ( !attrs ) return defvalue;
		for ( int i = 0; attrs[i] != 0; i += 2 ) 
			if ( wstos( attrs[i] ) == attname ) return wstos( attrs[ i + 1 ] );
		return defvalue;
	}
};

void startDocument( void *user_data ) 
{
	static_cast<CityGMLHandlerLibXml2*>(user_data)->startDocument();
}

void endDocument( void *user_data ) 
{
	static_cast<CityGMLHandlerLibXml2*>(user_data)->endDocument();
}

void startElement( void *user_data, const xmlChar *name, const xmlChar **attrs ) 
{
	static_cast<CityGMLHandlerLibXml2*>(user_data)->startElement( name, attrs );
}

void endElement( void *user_data, const xmlChar *name )
{
	static_cast<CityGMLHandlerLibXml2*>(user_data)->endElement( name );
}

void characters( void *user_data, const xmlChar *ch, int len )
{
	static_cast<CityGMLHandlerLibXml2*>(user_data)->characters( ch, len );
}

void fatalError( void *user_data, const char *msg, ... ) 
{
	std::string error = "Parsing error!";

	va_list args;
	va_start( args, msg );
#ifdef MSVC
	size_t len = _vscprintf( msg, args ) + 1;
	std::vector<char> buffer( len, '\0' );
	if ( _vsnprintf_s( &buffer[0], buffer.size(), len, msg, args ) ) error = &buffer[0];
#else
	size_t len = 1024;
	std::vector<char> buffer( len, '\0' );
	if ( vsnprintf( &buffer[0], buffer.size(), msg, args ) ) error = &buffer[0];
#endif
	va_end( args );

	static_cast<CityGMLHandlerLibXml2*>(user_data)->fatalError( error );
	throw new std::string( error );
}

// Parsing methods
namespace citygml
{
	CityModel* load( std::istream& stream, const ParserParams& params )
	{
		CityGMLHandlerLibXml2* handler = new CityGMLHandlerLibXml2( params );

		xmlSAXHandler sh = { 0 };
		sh.startDocument = startDocument;
		sh.endDocument = endDocument;
		sh.startElement = startElement;
		sh.endElement = endElement;
		sh.characters = characters;
		sh.error = fatalError;
		sh.fatalError = fatalError;

		xmlParserCtxtPtr context = xmlCreatePushParserCtxt( &sh, handler, 0, 0, "" );
		if ( !context ) 
		{
			std::cerr << "CityGML: Unable to create LibXml2 context!" << std::endl;
			delete handler;
			return 0;
		}	

		context->validate = 0;

		try 
		{ 
			// stream parsing
			std::string line;
			while ( std::getline( stream, line ) )
			{
				line += '\n';
				xmlParseChunk( context, line.c_str(), line.length(), 0 );
			}

			xmlParseChunk( context, 0, 0, 1 ); 		
		}
		catch ( ... ) 
		{
		}

		xmlFreeParserCtxt( context );

		CityModel* model = handler->getModel();

		delete handler;

		return model;	
	}

	CityModel* load( const std::string& fname, const ParserParams& params )
	{
		CityGMLHandlerLibXml2* handler = new CityGMLHandlerLibXml2( params );

		xmlSAXHandler sh = { 0 };
		sh.startDocument = startDocument;
		sh.endDocument = endDocument;
		sh.startElement = startElement;
		sh.endElement = endElement;
		sh.characters = characters;
		sh.error = fatalError;
		sh.fatalError = fatalError;


		xmlParserInputBufferPtr inputBuffer = xmlParserInputBufferCreateFilename( fname.c_str(), XML_CHAR_ENCODING_NONE );
		xmlParserCtxtPtr context = xmlCreateIOParserCtxt( &sh, handler, inputBuffer->readcallback, inputBuffer->closecallback, inputBuffer->context, XML_CHAR_ENCODING_NONE );
		if ( !context ) 
		{
			std::cerr << "CityGML: Unable to create LibXml2 context!" << std::endl;
			delete handler;
			return 0;
		}

		context->validate = 0;

		try 
		{ 
			xmlParseDocument( context );
		}
		catch ( ... ) 
		{
		}

		xmlFreeParserCtxt( context );

		CityModel* model = handler->getModel();

		delete handler;

		return model;	
	}
}

#endif
