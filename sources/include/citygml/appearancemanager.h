#pragma once

#include <memory>
#include <vector>

#include <citygml/object.h>
#include <citygml/vecs.hpp>

class Tesselator;

namespace citygml {

    class CityGMLLogger;
    class Appearance;
    class Material;
    class Texture;


    class AppearanceManager
    {
    public:
        AppearanceManager(std::shared_ptr<CityGMLLogger> logger );

        ~AppearanceManager();

        enum class ForSide
        {
            FS_ANY,	    // appearance for any side
            FS_FRONT,	// appearance for front side
            FS_BACK		// appearance for back side
        };

        /**
         * @brief returns the appearance with the given id
         * @return the Appearance object or nullptr if not found
         */
        std::shared_ptr<Appearance> getAppearanceByID( const std::string& id ) const;


        Tesselator& getTesselator();

        void finish();

    protected:
        std::string m_lastId;

        std::vector< std::shared_ptr<Appearance> > m_appearances;


        Tesselator m_tesselator;
        std::shared_ptr<CityGMLLogger> m_logger;
    };

}
