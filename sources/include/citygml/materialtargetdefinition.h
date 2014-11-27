#pragma once

#include <citygml/citygml_api.h>
#include <citygml/appearancetargetdefinition.h>
#include <citygml/material.h>

namespace citygml {

    class CityGMLFactory;

    /**
     * @brief The MaterialTargetDefinition associates a material with a target surface.
     */
    class LIBCITYGML_EXPORT MaterialTargetDefinition : public AppearanceTargetDefinition<Material> {
        friend class CityGMLFactory;
    protected:
        MaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id);

    };
}
