/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
 *
 * Contributors:
 *  - Manuel Garnier, BRGM - better normal computation
 *
 * This file is part of libcitygml library
 * http://code.google.com/p/libcitygml
 *
 * libcitygml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * libcitygml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 */

#include "citygml/citymodel.h"
#include "citygml/appearancemanager.h"
#include "citygml/appearance.h"
#include "citygml/citygmllogger.h"

#include <float.h>
#include <string.h>

#include <iterator>
#include <set>

#ifndef min
#	define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#endif

///////////////////////////////////////////////////////////////////////////////

namespace citygml
{
    CityModel::CityModel(const std::string& id) : Object( id )
    {

    }

    CityModel::~CityModel()
    {
        CityObjectsMap::const_iterator it = m_cityObjectsMap.begin();
        for ( ; it != m_cityObjectsMap.end(); ++it )
            for ( unsigned int i = 0; i < it->second.size(); i++ )
                delete it->second[i];

        for (AppearanceThemes::iterator appIt = m_appearanceThemes.begin(); appIt != m_appearanceThemes.end(); ++appIt)
        {
            if (appIt->second)
            {
                delete appIt->second;
                appIt->second = NULL;
            }
        }
    }

    const Envelope& CityModel::getEnvelope() const
    {
        return m_envelope;
    }

    const TVec3d& CityModel::getTranslationParameters() const
    {
        return m_translation;
    }

    unsigned int CityModel::size() const
    {
        unsigned int count = 0;
        CityObjectsMap::const_iterator it = m_cityObjectsMap.begin();
        for ( ; it != m_cityObjectsMap.end(); ++it ) count += it->second.size();
        return count;
    }

    const CityObjectsMap& CityModel::getCityObjectsMap() const
    {
        return m_cityObjectsMap;
    }

    const CityObjects* CityModel::getCityObjectsByType( CityObject::CityObjectsType type) const
    {
        CityObjectsMap::const_iterator it = m_cityObjectsMap.find( type );
        return ( it != m_cityObjectsMap.end() ) ? &it->second : 0;
    }

    const CityObjects& CityModel::getCityObjectsRoots() const
    {
        return m_roots;
    }

    const std::string& CityModel::getSRSName() const
    {
        return m_srsName;
    }

    AppearanceManager& CityModel::getOrCreateAppearanceManager(const std::string& theme, std::shared_ptr<citygml::CityGMLLogger> logger)
    {

    }

    void CityModel::addCityObject( CityObject* o )
    {
        CityObjectsMap::iterator it = m_cityObjectsMap.find( o->getType() );
        if ( it == m_cityObjectsMap.end() )
        {
            CityObjects v;
            v.push_back( o );
            m_cityObjectsMap[ o->getType() ] = v;
        }
        else
            it->second.push_back( o );
    }

    void CityModel::finish( const ParserParams& params, std::shared_ptr<CityGMLLogger> logger )
    {
        // get theme from params or use default theme
        std::string theme = params.theme.empty() ? getDefaultTheme() : params.theme;

        // make sure a valid was given
        if (m_appearanceThemes.find(theme) == m_appearanceThemes.end())
        {
            // fallback to default theme
            theme = getDefaultTheme();
        }

        AppearanceManager& appearanceManager = getOrCreateAppearanceManager(theme, logger);

        for(auto themePair : m_appearanceThemes) {
            CITYGML_LOG_DEBUG(logger, "Found theme: " << themePair.first << " (" << themePair.second->description() << ")");
        }


        // Assign appearances to cityobjects => geometries => polygons
        CityObjectsMap::const_iterator it = m_cityObjectsMap.begin();
        for ( ; it != m_cityObjectsMap.end(); ++it ) {
            for ( unsigned int i = 0; i < it->second.size(); i++ ) {
                it->second[i]->finish( appearanceManager, params );
            }
        }


        for (AppearanceThemes::iterator appIt = m_appearanceThemes.begin(); appIt != m_appearanceThemes.end(); ++appIt)
        {
            appIt->second->finish();
        }
    }

    std::shared_ptr<Appearance> CityModel::findAppearanceInAllThemes(const std::string& appearanceID)
    {
        for (const auto& appearanceThemePair : m_appearanceThemes) {

            std::shared_ptr<Appearance> result = appearanceThemePair.second->getAppearanceByID(appearanceID);

            if (result != nullptr) {
                return result;
            }

        }

        return nullptr;
    }

    std::string CityModel::getDefaultTheme() const
    {
        if (m_appearanceThemes.size() == 0)
        {
            return "";
        } else {
            return m_appearanceThemes.begin()->first;
        }
    }

    void CityModel::reassignNodeToAllAppearances(Polygon* polygon, std::vector<std::string>& cityGMLidStack)
    {
        for (auto appIt: m_appearanceThemes)
        {
            appIt.second->reassignNode(polygon->getId(), cityGMLidStack);
        }
    }

    std::ostream& operator<<( std::ostream& out, const CityModel& model )
    {
        const CityObjectsMap& cityObjectsMap = model.getCityObjectsMap();

        CityObjectsMap::const_iterator it = cityObjectsMap.begin();

        for ( ; it != cityObjectsMap.end(); ++it )

            for ( unsigned int i = 0; i < it->second.size(); i++ ) out << *(it->second[i]);

        out << model.size() << " city objects." << std::endl;

        return out;
    }

}
