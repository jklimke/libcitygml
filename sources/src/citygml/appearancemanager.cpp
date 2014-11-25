#include "citygml/appearancemanager.h"
#include "citygml/appearance.h"
#include "citygml/appearancetarget.h"
#include "citygml/citygmllogger.h"

namespace citygml {

    void AppearanceManager::addTheme(const std::string& theme)
    {
        m_themes.insert(theme);
    }

    void AppearanceManager::addAppearanceTarget(AppearanceTarget& target)
    {
        m_appearanceTargetsMap.insert(target.getId(), target);
    }

    void AppearanceManager::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        m_appearancesMap.insert(appearance->getId(), appearance);
    }

    void AppearanceManager::assignAppearancesToTargets()
    {
        for (std::pair<std::string, std::shared_ptr<Appearance>> appearancePair : m_appearancesMap) {

            std::vector<std::string> targetIDs = appearancePair.second->getTargetIDs();

            for (const std::string& targetID : targetIDs) {

                auto *it = m_appearanceTargetsMap.find(targetID);

                if (it == m_appearanceTargetsMap.end()) {
                    CITYGML_LOG_WARN(m_logger, "Appearance with id '" << appearancePair.second->getId() << "' targets object with id " << targetID << " but no such object exists.");
                    continue;
                }

                it->second.addAppearance(appearancePair.second);
            }

        }
    }

}
