//
//  externalreference.cpp
//  libcitygml
//
//  Created by 松本青空 on 2021/11/15.
//

#include <citygml/externalreference.h>

namespace citygml {
ExternalObjectReference::ExternalObjectReference() {
}

ExternalObjectReference::~ExternalObjectReference() {
}

ExternalReference::ExternalReference(std::string const& id) : Object(id) {
}
}
