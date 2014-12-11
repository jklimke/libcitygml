#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <parser/documentlocation.h>

#include <citygml/transformmatrix.h>
#include <citygml/citygmllogger.h>
#include <citygml/vecs.hpp>

namespace citygml {

    template<class T> inline T parseValue( const std::string &s, std::shared_ptr<citygml::CityGMLLogger>&, const DocumentLocation&)
    {
        std::stringstream ss;
        ss << s;
        T v;
        ss >> v;
        return v;
    }

    inline TransformationMatrix parseMatrix( const std::string &s, std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location)
    {
        std::stringstream ss;
        ss << s;


        double matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };

        for (size_t i = 0; i < 16; ++i)
        {
            if(ss.eof()) {
                CITYGML_LOG_WARN(logger, "Matrix with 16 elements expected, got '" << i + 1 << "' at " << location << ". Matrix may be invalid.");
                break;
            }

            ss >> matrix[i];
        }

        return TransformationMatrix(matrix);
    }

    template<> inline bool parseValue( const std::string &s, std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location )
    {
        // parsing a bool is special because "true" and "1" are true while "false" and "0" are false
        if (s == "1" || s == "true") {
            return true;
        } else if (s == "0" || s == "false") {
            return false;
        } else {
            CITYGML_LOG_WARN(logger, "Boolean expected, got '" << s << "' at " << location << " set value to false.");
        }
        return false;
    }

    template<class T> inline std::vector<T> parseVecList( const std::string &s,  std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location )
    {
        std::stringstream ss;
        ss << s;

        T v;
        std::vector<T> vec;
        while ( ss >> v )
            vec.push_back( v );

        if ( !ss.eof() )
        {
            CITYGML_LOG_WARN(logger, "Mismatch type, list of " << typeid(T).name() << " expected at " << location << " Ring/Polygon may be incomplete!");
        }

        return vec;
    }

    inline std::string parseReference(const std::string& reference, std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location) {
        if (reference.empty()) {
            CITYGML_LOG_WARN(logger, "Invalid reference value at " << location);
        }

        if (reference[0] == '#') {
            return reference.substr(1);
        } else {
            return reference;
        }
    }

}
