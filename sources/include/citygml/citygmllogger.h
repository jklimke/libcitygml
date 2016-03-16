#pragma once

#include <citygml/citygml_api.h>

#include <sstream>

namespace citygml {

    class LIBCITYGML_EXPORT CityGMLLogger {
    public:
        enum class LOGLEVEL {
            LL_ERROR = 4,
            LL_WARNING = 3,
            LL_INFO = 2,
            LL_DEBUG = 1,
            LL_TRACE = 0
        };

        CityGMLLogger(LOGLEVEL level = LOGLEVEL::LL_ERROR):m_logLevel(level){}

        /**
         * @brief logs a message. Might be called from different threads.
         */
        virtual void log(LOGLEVEL level, const std::string& message, const char* file=nullptr, int line=-1) const = 0;

        virtual bool isEnabledFor(LOGLEVEL level) const {
            return level >= getLogLevel();
        };

        virtual LOGLEVEL getLogLevel() const{
            return m_logLevel;
        };

        virtual LOGLEVEL setLogLevel(LOGLEVEL level) {
            return m_logLevel = level;
        };
    private:

        LOGLEVEL m_logLevel;
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
