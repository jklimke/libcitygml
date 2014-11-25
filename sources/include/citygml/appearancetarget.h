#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include <citygml/object.h>


namespace citygml {

    class Appearance;

    /**
     * @brief The AppearanceTarget class is the base class for all citygml objects that can be targets of appearances
     *
     * Ensures that each appearance is only added once and maintains the order of the appearances as they were added.
     */
    class AppearanceTarget : public citygml::Object {
    public:
        void addAppearance(std::shared_ptr<Appearance> appearance);
        void addAppearancesOf(const AppearanceTarget& other);

    protected:
        AppearanceTarget(const std::string& id);
        std::vector<std::shared_ptr<Appearance>> m_appearances;

    private:
        std::unordered_set<std::shared_ptr<Appearance>> m_appearancesSet;
    };
}
