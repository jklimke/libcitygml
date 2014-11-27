#include "citygml/appearancetarget.h"
#include "citygml/texturetargetdefinition.h"
#include "citygml/materialtargetdefinition.h"
#include "citygml/appearancetargetdefinition.h"
#include "citygml/appearance.h"

#include <algorithm>

namespace citygml {

    AppearanceTarget::AppearanceTarget(const std::string& id) : Object(id)
    {

    }


    void AppearanceTarget::addTargetDefinition(std::shared_ptr<AppearanceTargetDefinition<Appearance> > targetDef)
    {
        if (targetDef->getAppearance()->asMaterial() != nullptr) {
            addTargetDefinition(std::dynamic_pointer_cast<MaterialTargetDefinition>(targetDef));
        } else {
            addTargetDefinition(std::dynamic_pointer_cast<TextureTargetDefinition>(targetDef));
        }
    }

    void AppearanceTarget::addTargetDefinition(std::shared_ptr<TextureTargetDefinition> targetDef)
    {
        for (const std::string& theme : targetDef->getAppearance()->getThemes()) {
            if (m_themeTexMap.count(theme) == 0) {
                m_themeTexMap[theme] = targetDef;
            }
        }
    }

    void AppearanceTarget::addTargetDefinition(std::shared_ptr<MaterialTargetDefinition> targetDef)
    {
        for (const std::string& theme : targetDef->getAppearance()->getThemes()) {
            if (m_themeMatMap.count(theme) == 0) {
                m_themeMatMap[theme] = targetDef;
            }
        }
    }

    void AppearanceTarget::addTargetDefinitionsOf(const AppearanceTarget& other)
    {
        // Insert the TargetDefinitions of the other AppearanceTarget that belong to a themes for which this AppearanceTarget contains no TargetDefinitions
        m_themeMatMap.insert(other.m_themeMatMap.begin(), other.m_themeMatMap.end());
        m_themeTexMap.insert(other.m_themeTexMap.begin(), other.m_themeTexMap.end());
    }

    MaterialTargetDefinition* AppearanceTarget::getMaterialTargetDefinitionForTheme(const std::string& theme)
    {
        auto it = m_themeMatMap.find(theme);
        if (it == m_themeMatMap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    const MaterialTargetDefinition* AppearanceTarget::getMaterialTargetDefinitionForTheme(const std::string& theme) const
    {
        const auto it = m_themeMatMap.find(theme);
        if (it == m_themeMatMap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    TextureTargetDefinition* AppearanceTarget::getTextureTargetDefinitionForTheme(const std::string& theme)
    {
        auto it = m_themeTexMap.find(theme);
        if (it == m_themeTexMap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    const TextureTargetDefinition* AppearanceTarget::getTextureTargetDefinitionForTheme(const std::string& theme) const
    {
        const auto it = m_themeTexMap.find(theme);
        if (it == m_themeTexMap.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    std::vector<TextureTargetDefinition*> AppearanceTarget::getTextureTargetDefinitions()
    {
        std::vector<TextureTargetDefinition*> texTargetDefs;
        for (auto& pair : m_themeTexMap) {
            texTargetDefs.push_back(pair.second.get());
        }
        return texTargetDefs;
    }



}
