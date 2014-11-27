#include "citygml/appearancemanager.h"
#include "citygml/appearance.h"
#include "citygml/appearancetarget.h"
#include "citygml/citygmllogger.h"
#include "citygml/materialtargetdefinition.h"
#include "citygml/texturetargetdefinition.h"

namespace citygml {

    void AppearanceManager::addTheme(const std::string& theme)
    {
        m_themes.insert(theme);
    }

    void AppearanceManager::addAppearanceTarget(AppearanceTarget* target)
    {
        m_appearanceTargetsMap[target->getId()] = target;
    }

    void AppearanceManager::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        m_appearancesMap[appearance->getId()] = appearance;
    }

    void AppearanceManager::addTextureTargetDefinition(std::shared_ptr<TextureTargetDefinition> targetDef)
    {
        m_texTargetDefinitions.push_back(targetDef);
    }

    void AppearanceManager::addMaterialTargetDefinition(std::shared_ptr<MaterialTargetDefinition> targetDef)
    {
        m_materialTargetDefinitions.push_back(targetDef);
    }

    template<class T> void assignTargetDefinition(std::shared_ptr<T>& targetDef, std::unordered_map<std::string, AppearanceTarget*> targetMap, std::shared_ptr<CityGMLLogger>& logger) {
        std::string targetID = targetDef->getTargetID();
        auto it = targetMap.find(targetID);

        if (it == targetMap.end()) {
            CITYGML_LOG_WARN(logger, "Appearance with id '" << targetDef->getAppearance()->getId() << "' targets object with id " << targetID << " but no such object exists.");
        } else {
            it->second->addTargetDefinition(targetDef);
        }
    }

    void AppearanceManager::assignAppearancesToTargets()
    {
        for (std::shared_ptr<MaterialTargetDefinition>& targetDef : m_materialTargetDefinitions ) {
            assignTargetDefinition<MaterialTargetDefinition>(targetDef, m_appearanceTargetsMap, m_logger);
        }

        for (std::shared_ptr<TextureTargetDefinition>& targetDef : m_texTargetDefinitions ) {
            assignTargetDefinition<TextureTargetDefinition>(targetDef, m_appearanceTargetsMap, m_logger);
        }
    }

}
