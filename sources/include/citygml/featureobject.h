#pragma once

#include <memory>

#include "citygml/citygml_api.h"
#include "citygml/object.h"

namespace citygml {

    class Envelope;

    class FeatureObject : public Object {
    public:
        FeatureObject(const std::string& gmlID);

        const Envelope& getEnvelope() const;
        void setEnvelope(Envelope* e);

        virtual ~FeatureObject();

    protected:
        std::unique_ptr<Envelope> m_envelope;
    };

}
