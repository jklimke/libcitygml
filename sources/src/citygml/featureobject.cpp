#include <citygml/featureobject.h>

#include <citygml/envelope.h>

namespace citygml {

    FeatureObject::FeatureObject(const std::string& gmlID) : Object(gmlID)
    {
        setEnvelope(new Envelope());
    }

    const Envelope& FeatureObject::getEnvelope() const
    {
        return *m_envelope;
    }

    void FeatureObject::setEnvelope(Envelope* e)
    {
        m_envelope = std::unique_ptr<Envelope>(e);
    }

    FeatureObject::~FeatureObject()
    {

    }

}
