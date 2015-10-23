#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>
#include <citygml/appearancetarget.h>
#include <citygml/citygmllogger.h>
#include <citygml/materialtargetdefinition.h>
#include <citygml/texturetargetdefinition.h>

namespace citygml {

    AppearanceManager::AppearanceManager(std::shared_ptr<CityGMLLogger> logger)
    {
        m_logger = logger;
    }

    AppearanceManager::~AppearanceManager()
    {

    }

    std::shared_ptr<Appearance> AppearanceManager::getAppearanceByID(const std::string& id) const
    {
        auto it = m_appearancesMap.find(id);

        if (it == m_appearancesMap.end()) {
            return nullptr;
        }

        return it->second;
    }

    std::vector<std::string> AppearanceManager::getAllThemes()
    {
        std::vector<std::string> themes(m_themes.begin(), m_themes.end());
        return themes;
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

    template<class T> void assignTargetDefinition(std::shared_ptr<T>& targetDef, const std::unordered_map<std::string, AppearanceTarget*>& targetMap, std::shared_ptr<CityGMLLogger>& logger) {
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
        CITYGML_LOG_INFO(m_logger, "Start assignment of appearances to targets ("
                         << m_materialTargetDefinitions.size() << " material target definition(s), "
                         << m_texTargetDefinitions.size() << " texture target definition(s)).");

        for (std::shared_ptr<MaterialTargetDefinition>& targetDef : m_materialTargetDefinitions ) {
            assignTargetDefinition<MaterialTargetDefinition>(targetDef, m_appearanceTargetsMap, m_logger);
            addThemesFrom(targetDef->getAppearance());
        }

        for (std::shared_ptr<TextureTargetDefinition>& targetDef : m_texTargetDefinitions ) {
            assignTargetDefinition<TextureTargetDefinition>(targetDef, m_appearanceTargetsMap, m_logger);
            addThemesFrom(targetDef->getAppearance());
        }

        m_materialTargetDefinitions.clear();
        m_texTargetDefinitions.clear();
        m_appearanceTargetsMap.clear();
        m_appearancesMap.clear();

        CITYGML_LOG_INFO(m_logger, "Finished assignment of appearances to targets ("
                         << m_materialTargetDefinitions.size() << " material target definition(s), "
                         << m_texTargetDefinitions.size() << " texture target definition(s)).");

    }

    void AppearanceManager::addThemesFrom(std::shared_ptr<Appearance> surfaceData)
    {
        m_themes.insert(surfaceData->getThemes().begin(), surfaceData->getThemes().end());
    }

}
