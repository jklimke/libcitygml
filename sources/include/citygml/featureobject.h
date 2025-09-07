#pragma once

#include <memory>

#include <citygml/object.h>
#include <citygml/envelope.h>
#include <citygml/warnings.h>

namespace citygml {

    class Envelope;

    class LIBCITYGML_EXPORT FeatureObject : public Object {
    public:
        FeatureObject(const std::string& gmlID);

        const Envelope& getEnvelope() const;
        void setEnvelope(Envelope* e);

        ~FeatureObject() override;

    protected:
        PRAGMA_WARN_DLL_BEGIN
        std::unique_ptr<Envelope> m_envelope;
        PRAGMA_WARN_DLL_END
    };

}
