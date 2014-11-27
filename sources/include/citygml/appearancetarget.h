#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <citygml/object.h>
#include <citygml/appearancetargetdefinition.h>


namespace citygml {

    class MaterialTargetDefinition;
    class TextureTargetDefinition;
    class Appearance;

    /**
     * @brief The AppearanceTarget class is the base class for all citygml objects that can be targets of appearances
     *
     * Ensures that there is only one texture and material per theme
     */
    class AppearanceTarget : public citygml::Object {
    public:

        void addTargetDefinition(std::shared_ptr<AppearanceTargetDefinition<Appearance>> targetDef);
        void addTargetDefinition(std::shared_ptr<TextureTargetDefinition> targetDef);
        void addTargetDefinition(std::shared_ptr<MaterialTargetDefinition> targetDef);

        void addTargetDefinitionsOf(const AppearanceTarget& other);

        MaterialTargetDefinition* getMaterialTargetDefinitionForTheme(const std::string& theme);
        const MaterialTargetDefinition* getMaterialTargetDefinitionForTheme(const std::string& theme) const;

        TextureTargetDefinition* getTextureTargetDefinitionForTheme(const std::string& theme);
        const TextureTargetDefinition* getTextureTargetDefinitionForTheme(const std::string& theme) const;

        std::vector<TextureTargetDefinition*> getTextureTargetDefinitions();

    protected:
        AppearanceTarget(const std::string& id);



    private:
        std::unordered_map<std::string, std::shared_ptr<MaterialTargetDefinition>> m_themeMatMap;
        std::unordered_map<std::string, std::shared_ptr<TextureTargetDefinition>> m_themeTexMap;

    };
}
