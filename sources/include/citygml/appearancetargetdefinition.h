#pragma once

#include <memory>
#include <string>

#include <citygml/object.h>

namespace citygml {

    /*
     * TODO:
     *  - store the target id here
     *  - store the appearance object here as a shared pointer
     *  - create a subclass for material appearance type and texture appearance type (TextureTargetDefinition, MaterialTargetDefinition)
     *  - modify AppearanceTarget so that is contains distinct lists for TextureTargetDefinition and MaterialTargetDefinition definitions... store the AppearanceTargetDefinitions as shared_ptr
     *  - remove all knowledge of thier targets from the appearance objects (Texture, Material)
     *  - let the appearance manager store the AppearanceTargetDefinition objects and the Appearance objects until they are assigned... then remove them from the appearance manager
     *  - when finishing the AppearanceTarget objcts pass down the AppearanceTargetDefinition
     *  - when finishing the polygon object remove all AppearanceTargetDefinition that are unused
     */

    /**
     * @brief defines the association between an Appearance object and an AppearanceTarget object
     */
    template<class T>
    class LIBCITYGML_EXPORT AppearanceTargetDefinition : public Object {
    public:
        AppearanceTargetDefinition(const std::string& targetID, std::shared_ptr<T> appearance, const std::string& id) : Object(id), m_targetID(targetID), m_appearance(appearance) {}

        /**
         * @brief the id of the target surface
         * @note The targetID must not be the id of the object on which the texture is applied at the end (may be passed down to the children of the target object)
         */
        std::string getTargetID() const { return m_targetID; }

        std::shared_ptr<const T> getAppearance() const { return m_appearance; }
        std::shared_ptr<T> getAppearance() { return m_appearance; }

        virtual ~AppearanceTargetDefinition() {}

    protected:
        std::string m_targetID;
        std::shared_ptr<T> m_appearance;
    };

}
