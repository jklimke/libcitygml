#pragma once

#include <string>
#include <ostream>
#include <stdint.h>

namespace citygml {

    class DocumentLocation {
    public:
        virtual const std::string& getDocumentFileName() const = 0;
        virtual const std::string& getDocumentFilePath() const = 0;
        virtual uint64_t getCurrentLine() const = 0;
        virtual uint64_t getCurrentColumn() const = 0;
    };

    inline std::ostream& operator<<( std::ostream& os, const DocumentLocation& o ) {

        if (o.getCurrentLine() == 0) {
            os << " unknown location";
        } else {
            os << " line " << o.getCurrentLine();

            if (o.getCurrentColumn() > 0) {
                os << ", column " << o.getCurrentColumn();
            }
        }

        if (!o.getDocumentFileName().empty()) {
            os << " in file " << o.getDocumentFileName();
        }

        return os;

    }
}
