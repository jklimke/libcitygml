#include "citygml/materialtargetdefinition.h"

namespace citygml {

    MaterialTargetDefinition::MaterialTargetDefinition(const std::string& targetID, std::shared_ptr<Material> appearance, const std::string& id) : AppearanceTargetDefinition(targetID, appearance, id)
    {

    }

}
