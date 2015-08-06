#include "citygml/appearancetarget.h"
#include "citygml/texturetargetdefinition.h"
#include "citygml/materialtargetdefinition.h"
#include "citygml/appearancetargetdefinition.h"
#include "citygml/appearance.h"

#include <unordered_set>
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
            if (targetDef->getAppearance()->getIsFront() && m_themeTexMapFront.count(theme) == 0) {
                m_themeTexMapFront[theme] = targetDef;
            } else if (!targetDef->getAppearance()->getIsFront() && m_themeTexMapBack.count(theme) == 0) {
                m_themeTexMapBack[theme] = targetDef;
            }
        }
    }

    void AppearanceTarget::addTargetDefinition(std::shared_ptr<MaterialTargetDefinition> targetDef)
    {
        for (const std::string& theme : targetDef->getAppearance()->getThemes()) {
            if (targetDef->getAppearance()->getIsFront() && m_themeMatMapFront.count(theme) == 0) {
                m_themeMatMapFront[theme] = targetDef;
            } else if (!targetDef->getAppearance()->getIsFront() && m_themeMatMapBack.count(theme) == 0) {
                m_themeMatMapBack[theme] = targetDef;
            }
        }
    }

    void AppearanceTarget::addTargetDefinitionsOf(const AppearanceTarget& other)
    {
        // Insert the TargetDefinitions of the other AppearanceTarget that belong to a themes for which this AppearanceTarget contains no TargetDefinitions
        m_themeMatMapFront.insert(other.m_themeMatMapFront.begin(), other.m_themeMatMapFront.end());
        m_themeMatMapBack.insert(other.m_themeMatMapBack.begin(), other.m_themeMatMapBack.end());

        m_themeTexMapFront.insert(other.m_themeTexMapFront.begin(), other.m_themeTexMapFront.end());
        m_themeTexMapBack.insert(other.m_themeTexMapBack.begin(), other.m_themeTexMapBack.end());
    }

    std::shared_ptr<MaterialTargetDefinition> AppearanceTarget::getMaterialTargetDefinitionForTheme(const std::string& theme, bool front)
    {
        auto& map = front ? m_themeMatMapFront : m_themeMatMapBack;
        auto it = map.find(theme);
        if (it == map.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::shared_ptr<const MaterialTargetDefinition> AppearanceTarget::getMaterialTargetDefinitionForTheme(const std::string& theme, bool front) const
    {
        auto& map = front ? m_themeMatMapFront : m_themeMatMapBack;
        const auto it = map.find(theme);
        if (it == map.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::shared_ptr<TextureTargetDefinition> AppearanceTarget::getTextureTargetDefinitionForTheme(const std::string& theme, bool front)
    {
        auto& map = front ? m_themeTexMapFront : m_themeTexMapBack;
        auto it = map.find(theme);
        if (it == map.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::shared_ptr<const TextureTargetDefinition> AppearanceTarget::getTextureTargetDefinitionForTheme(const std::string& theme, bool front) const
    {
        auto& map = front ? m_themeTexMapFront : m_themeTexMapBack;
        const auto it = map.find(theme);
        if (it == map.end()) {
            return nullptr;
        }
        return it->second;
    }

    std::vector<TextureTargetDefinition*> AppearanceTarget::getTextureTargetDefinitions()
    {
        std::unordered_set<TextureTargetDefinition*> texTargetDefs;
        for (auto& pair : m_themeTexMapFront) {
            texTargetDefs.insert(pair.second.get());
        }
        for (auto& pair : m_themeTexMapBack) {
            texTargetDefs.insert(pair.second.get());
        }
        return std::vector<TextureTargetDefinition*>(texTargetDefs.begin(), texTargetDefs.end());
    }

    std::vector<std::string> AppearanceTarget::getAllTextureThemes(bool front) const
    {
        auto& map = front ? m_themeTexMapFront : m_themeTexMapBack;
        std::vector<std::string> themes;
        for (const auto& pair : map) {
            themes.push_back(pair.first);
        }
        return themes;
    }



}
