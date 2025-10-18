#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include <parser/documentlocation.h>

#include <citygml/transformmatrix.h>
#include <citygml/citygmllogger.h>
#include <citygml/vecs.hpp>

namespace citygml {

    template <typename T, std::enable_if_t<std::is_fundamental_v<T>, bool> = true>
    char const* readNextValue(std::string_view view, T& target) {
        char const* end;
        std::tie(target, end) = readNextNumber<T>(view);
        return end;
    }

    template <typename T, std::enable_if_t<!std::is_fundamental_v<T>, bool> = true>
    char const* readNextValue(std::string_view view, T& target) {
         return target.fromString(view);
    }

    template<class T> inline T parseValue( const std::string &s, std::shared_ptr<citygml::CityGMLLogger>&, const DocumentLocation&)
    {
        T value;
        readNextValue<T>(s, value);
        return value;
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

    template<class T>
    inline std::vector<T> parseVecList( const std::string &s,  std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location )
    {
        if (std::all_of(s.begin(), s.end(), shouldSkip)) {
            return {};
        }

        std::string_view view(s);
        std::vector<T> vec;
        while (!view.empty()) {
            T value;
            char const* end = readNextValue(view, value);
            std::ptrdiff_t const consumedChars = std::distance(view.data(), end);
            if (consumedChars > 0) {
                vec.push_back(value);
                view = view.substr(consumedChars);
            } else {
                break; // reading stopped at an invalid character
            }
        }
        return vec;
    }

    inline TransformationMatrix parseMatrix( const std::string &s, std::shared_ptr<citygml::CityGMLLogger>& logger, const DocumentLocation& location)
    {
        std::string_view view(s);

        double matrix[16] = { 1.0, 0.0, 0.0, 0.0,
                              0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 1.0 };
        for (size_t i = 0; i < 16; ++i){
            if(view.empty() || std::all_of(view.begin(), view.end(), shouldSkip)) {
                CITYGML_LOG_WARN(logger, "Matrix with 16 elements expected, got '" << i + 1 << "' at " << location << ". Matrix may be invalid.");
                break;
            }
            char const* next = readNextValue(view, matrix[i]);
            view = view.substr(std::distance(view.data(), next));
        }
        return TransformationMatrix(matrix);
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
