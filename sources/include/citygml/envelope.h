#pragma once

#include <iostream>

#include <citygml/citygml_api.h>
#include <citygml/vecs.hpp>

namespace citygml {

    /**
     * @brief The Envelope class defines a bounding box in an spatial reference system (gml:Envelope)
     */
    class LIBCITYGML_EXPORT Envelope
    {
    public:
        Envelope();
        Envelope(const std::string& srsName);

        /**
         * @brief lower left front corner of the bounding box in srs coordinates
         */
        const TVec3d& getLowerBound() const;
        void setLowerBound(const TVec3d& coordinate);

        /**
         * @brief upper right back corner of the bounding box in srs coordinates
         */
        const TVec3d& getUpperBound() const;
        void setUpperBound(const TVec3d& coordinate);

        /**
         * @brief the name of the spatial reference system
         */
        const std::string& srsName() const;

        const bool validBounds() const;

    protected:
        TVec3d m_lowerBound;
        TVec3d m_upperBound;
        std::string m_srsName;
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream&, const citygml::Envelope& );
}
