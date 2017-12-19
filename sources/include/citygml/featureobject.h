#pragma once

#include <memory>

#include <citygml/object.h>
#include <citygml/envelope.h>

namespace citygml {

    class Envelope;

    class LIBCITYGML_EXPORT FeatureObject : public Object {
    public:
        FeatureObject(const std::string& gmlID);

        const Envelope& getEnvelope() const;
        void setEnvelope(Envelope* e);

        virtual ~FeatureObject();

    protected:
        std::unique_ptr<Envelope> m_envelope;
    };

}
