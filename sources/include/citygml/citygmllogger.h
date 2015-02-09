#pragma once

#include <citygml/citygml_api.h>

#include <sstream>

namespace citygml {

    class LIBCITYGML_EXPORT CityGMLLogger {
    public:
        enum class LOGLEVEL {
            LL_ERROR,
            LL_WARNING,
            LL_INFO,
            LL_DEBUG,
            LL_TRACE
        };

        /**
         * @brief logs a message. Might be called from different threads.
         */
        virtual void log(LOGLEVEL level, const std::string& message, const char* file=nullptr, int line=-1) const = 0;

        virtual bool isEnabledFor(LOGLEVEL level) const = 0;

    };

    /**
      * @brief logs a message for a certain log level
      * @param logger a pointer to a CityGMLLogger
      * @param level the CityGMLLogger::LOGLEVEL
      * @param message a string or a stream expression
      */
    #define CITYGML_LOG(logger, level, message)                             \
        do {                                                                \
            if (logger->isEnabledFor(level)) {                              \
                std::stringstream ss;                                       \
                ss << message;                                              \
                logger->log(level, ss.str(), __FILE__,  __LINE__);          \
            }                                                               \
        } while (0);


    #define CITYGML_LOG_ERROR(logger, message) CITYGML_LOG(logger, citygml::CityGMLLogger::LOGLEVEL::LL_ERROR, message)
    #define CITYGML_LOG_WARN(logger, message) CITYGML_LOG(logger, citygml::CityGMLLogger::LOGLEVEL::LL_WARNING, message)
    #define CITYGML_LOG_INFO(logger, message) CITYGML_LOG(logger, citygml::CityGMLLogger::LOGLEVEL::LL_INFO, message)
    #define CITYGML_LOG_DEBUG(logger, message) CITYGML_LOG(logger, citygml::CityGMLLogger::LOGLEVEL::LL_DEBUG, message)
    #define CITYGML_LOG_TRACE(logger, message) CITYGML_LOG(logger, citygml::CityGMLLogger::LOGLEVEL::LL_TRACE, message)
}
