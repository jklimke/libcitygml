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
    class MaterialTargetDefinition;
    class TextureTargetDefinition;


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
         * @note should be called after assignAppearancesToTargets (otherwise the list will be empty)
         */
        std::vector<std::string> getAllThemes();

        void addAppearanceTarget(AppearanceTarget* target);

        void addAppearance(std::shared_ptr<Appearance> appearance);
        void addTextureTargetDefinition(std::shared_ptr<TextureTargetDefinition> targetDef);
        void addMaterialTargetDefinition(std::shared_ptr<MaterialTargetDefinition> targetDef);

        /**
         * @brief assigns each appearance to all targets for which a coresponding AppearanceTargetDefinition exits.
         * @note should be called once after parsing has finished
         */
        void assignAppearancesToTargets();


    protected:
        std::unordered_map<std::string, std::shared_ptr<Appearance> > m_appearancesMap;
        std::vector<std::shared_ptr<MaterialTargetDefinition> > m_materialTargetDefinitions;
        std::vector<std::shared_ptr<TextureTargetDefinition> > m_texTargetDefinitions;
        std::unordered_set<std::string> m_themes;
        std::unordered_map<std::string, AppearanceTarget*> m_appearanceTargetsMap;
        std::shared_ptr<CityGMLLogger> m_logger;

        void addThemesFrom(std::shared_ptr<Appearance> surfaceData);
    };

}
