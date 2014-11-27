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
#include <algorithm>

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

    void CityModel::addToCityObjectsMapRecursive(const CityObject* cityObj)
    {

        CityObjectsMap::iterator it = m_cityObjectsMap.find(cityObj->getType());

        if (it == m_cityObjectsMap.end()) {
            m_cityObjectsMap[cityObj->getType()] = std::vector<const CityObject*>({cityObj});
        } else {
            it->second.push_back(cityObj);
        }

        for (int i = 0; i < cityObj->getChildCityObjecsCount(); i++) {
            addToCityObjectsMapRecursive(&cityObj->getChildCityObject(i));
        }
    }
    std::vector<std::string> CityModel::themes() const
    {
        return m_themes;
    }

    void CityModel::setThemes(const std::vector<std::string>& themes)
    {
        m_themes = themes;
    }


    CityModel::~CityModel()
    {
    }

    const Envelope& CityModel::getEnvelope() const
    {
        return m_envelope;
    }

    void CityModel::setEnvelope(Envelope e)
    {
        m_envelope = e;
    }

    const TVec3d& CityModel::getTranslationParameters() const
    {
        return m_translation;
    }

    void CityModel::setTranslationParameters(const TVec3d& param)
    {
        m_translation = param;
    }


    const ConstCityObjects CityModel::getAllCityObjectsOfType( CityObject::CityObjectsType type ) const
    {
        CityObjectsMap::const_iterator it = m_cityObjectsMap.find( type );
        return it->second;
    }

    const ConstCityObjects CityModel::getRootCityObjects() const
    {
        ConstCityObjects list;
        std::transform(m_roots.begin(), m_roots.end(), list.begin(), [](const std::unique_ptr<CityObject>& cityObj) {
            return cityObj.get();
        });
        return list;
    }

    const std::string& CityModel::getSRSName() const
    {
        return m_srsName;
    }


    void CityModel::finish(bool tesselate, Tesselator& tesselator, bool mergePolygons )
    {
        // Apply appearances to targets

        // Finish all cityobjcts

        // Build city objects map
        for (std::unique_ptr<CityObject>& obj : m_roots) {
            addToCityObjectsMapRecursive(obj.get());
        }
    }

    std::ostream& operator<<( std::ostream& out, const CityModel& model )
    {
        out << "Root CityObjects: " << std::endl;
        ConstCityObjects rootObjcts = model.getRootCityObjects();
        for (const CityObject* cityObj : rootObjcts) {
            out << cityObj << std::endl;
        }
        return out;
    }

}
