#include <citygml/externalreference.h>

namespace citygml {
    namespace {
        const std::string EMPTY_STRING("");
    } // anonymous namespace

    const std::string& ExternalObjectReference::getName() const {
        if (type == ObjectRefType::NAME) {
            return value;
        } else {
            return EMPTY_STRING;
        }
    }

    const std::string& ExternalObjectReference::getUri() const {
        if (type == ObjectRefType::URI) {
            return value;
        } else {
            return EMPTY_STRING;
        }
    }

    void ExternalObjectReference::setName(const std::string& name) {
        type = ObjectRefType::NAME;
        value = name;
    }

    void ExternalObjectReference::setUri(const std::string& uri) {
        type = ObjectRefType::URI;
        value = uri;
    }

    ExternalReference::ExternalReference(std::string const& id) : Object(id) {
    }
}
