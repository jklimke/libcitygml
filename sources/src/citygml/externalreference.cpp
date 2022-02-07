#include <citygml/externalreference.h>

namespace citygml {
    ExternalObjectReference::ExternalObjectReference() {
    }

    ExternalObjectReference::~ExternalObjectReference() {
    }

    ExternalReference::ExternalReference(std::string const& id) : Object(id) {
    }
}
