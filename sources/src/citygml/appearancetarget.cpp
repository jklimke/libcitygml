#include "citygml/appearancetarget.h"
#include "citygml/appearance.h"

#include <algorithm>

namespace citygml {

    AppearanceTarget::AppearanceTarget(const std::string& id) : Object(id)
    {

    }

    void AppearanceTarget::addAppearance(std::shared_ptr<Appearance> appearance)
    {
        if (m_appearancesSet.count(appearance) == 0) {
            m_appearances.push_back(appearance);
            m_appearancesSet.insert();
        }
    }

    void AppearanceTarget::addAppearancesOf(const AppearanceTarget& other)
    {
        std::unordered_set<std::shared_ptr<Appearance>> diff;

        std::set_difference(other.m_appearancesSet.begin(), other.m_appearancesSet.end(), this->m_appearancesSet.begin(), this->m_appearancesSet.end(), std::inserter(diff, diff.end()));

        for (std::shared_ptr<Appearance> appearance : diff) {
            appearance->copyTargetDefinition(other, this->getId());
        }

        m_appearancesSet.insert(diff.begin(), diff.end());
        m_appearances.insert(m_appearances.end(), diff.begin(), diff.end());
    }



}
