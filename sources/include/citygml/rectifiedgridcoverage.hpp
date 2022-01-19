//
//  rectifiedgridcoverage.hpp
//  citygml
//
//  Created by 松本青空 on 2021/10/29.
//

#pragma once

#include <citygml/featureobject.h>

namespace citygml {

    class LIBCITYGML_EXPORT RectifiedGridCoverage : public FeatureObject
    {
        friend class CityGMLFactory;
        
    protected:
        RectifiedGridCoverage(std::string const& id = "RectifiedGridCoverage");
    };

    LIBCITYGML_EXPORT std::ostream& operator<<( std::ostream& os, const RectifiedGridCoverage& o );

}

