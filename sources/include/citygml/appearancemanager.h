#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <citygml/object.h>
#include <citygml/vecs.hpp>

class Tesselator;

namespace citygml {

    class CityGMLLogger;
    class Appearance;
    class AppearanceTarget;
    class Material;
    class Texture;


    class AppearanceManager
    {
    public:
        AppearanceManager(std::shared_ptr<CityGMLLogger> logger );

        ~AppearanceManager();

        /**
         * @brief returns the appearance with the given id
         * @return the Appearance object or nullptr if not found
         */
        std::shared_ptr<Appearance> getAppearanceByID( const std::string& id ) const;

        /**
         * @brief all themes found in the parsed citygml file
         * @return a list of theme identifiers
         */
        std::vector<std::string> getAllThemes();

        void addTheme(const std::string& theme);

        void addAppearanceTarget(AppearanceTarget& target);

        void addAppearance(std::shared_ptr<Appearance> appearance);

        void assignAppearancesToTargets();


    protected:
        std::unordered_map<std::string, std::shared_ptr<Appearance>> m_appearancesMap;
        std::unordered_set<std::string> m_themes;
        std::unordered_map<std::string, AppearanceTarget&> m_appearanceTargetsMap;
        std::shared_ptr<CityGMLLogger> m_logger;
    };

}
