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

#include <filesystem>
#include <citygml/citygml.h>
#include <citygml/citygmllogger.h>

#include <fstream>
#include <string>
#include <memory>
#include <mutex>

#include <citygml/citygml_api.h>
#include "parser/citygmldocumentparser.h"
#include "parser/documentlocation.h"
#include "parser/attributes.h"

using namespace citygml;

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
    bool xerces_initialized;

    std::shared_ptr<const CityModel> load(std::istream& stream, const ParserParams& params, std::shared_ptr<CityGMLLogger> logger)
    {
        return nullptr;
    }

    std::shared_ptr<const CityModel> load( const std::string& fname, const ParserParams& params , std::shared_ptr<CityGMLLogger> logger)
    {
        return nullptr;
    }
}

