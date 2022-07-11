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

#include <citygml/citymodel.h>
#include <citygml/appearancemanager.h>
#include <citygml/appearance.h>
#include <citygml/citygmllogger.h>

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
    CityModel::CityModel(const std::string& id) : FeatureObject( id )
    {

    }

    void CityModel::addToCityObjectsMapRecursive(const CityObject* cityObj)
    {

        CityObjectsMap::iterator it = m_cityObjectsMap.find(cityObj->getType());

        if (it == m_cityObjectsMap.end()) {
            std::vector<const CityObject*> tmp;
            tmp.push_back(cityObj);
            m_cityObjectsMap[cityObj->getType()] = std::vector<const CityObject*>(tmp);
        } else {
            it->second.push_back(cityObj);
        }

        for (int i = 0; i < cityObj->getChildCityObjectsCount(); i++) {
            addToCityObjectsMapRecursive(&cityObj->getChildCityObject(i));
        }
    }

    void CityModel::addToIdToCityObjMapRecursive(const CityObject *cityObj) {
        m_idToCityObjMap[cityObj->getId()] = cityObj;
        for (int i = 0; i < cityObj->getChildCityObjectsCount(); i++ ) {
            addToIdToCityObjMapRecursive(&cityObj->getChildCityObject(i));
        }
    }

    std::vector<std::string> CityModel::themes() const
    {
        return m_themes;
    }

    void CityModel::setThemes(std::vector<std::string> themes)
    {
        m_themes = themes;
    }


    CityModel::~CityModel()
    {
    }

    const ConstCityObjects CityModel::getAllCityObjectsOfType( CityObject::CityObjectsType type ) const
    {
        std::vector<CityObject::CityObjectsType> keys;
        for (const auto& [key, _] : m_cityObjectsMap) {
            if (static_cast<uint64_t>(key & type) != 0ull) {
                keys.push_back(key);
            }
        }

        if (keys.empty())
            return {};

        ConstCityObjects result;
        for (const auto& key : keys) {
            const auto city_objects = m_cityObjectsMap.find(key)->second;
            result.insert(result.end(), city_objects.begin(), city_objects.end());
        }
        return result;
    }

    const CityObject* CityModel::getCityObjectById(const std::string& id) const {
        if(m_idToCityObjMap.count(id) == 0){
            return nullptr;
        }
        auto foundObj = m_idToCityObjMap.at(id);
        return foundObj;
    }

    const ConstCityObjects CityModel::getRootCityObjects() const
    {
        ConstCityObjects list;
        for (const std::unique_ptr<CityObject>& cityObj : m_roots) {
            list.push_back(cityObj.get());
        }
        return list;
    }

    void CityModel::addRootObject(CityObject* obj)
    {
        m_roots.push_back(std::unique_ptr<CityObject>(obj));
    }

    unsigned int CityModel::getNumRootCityObjects() const
    {
        return m_roots.size();
    }

    CityObject& CityModel::getRootCityObject(int i)
    {
        return *m_roots.at(i);
    }

    const CityObject& CityModel::getRootCityObject(int i) const
    {
        return *m_roots.at(i);
    }

    const std::string& CityModel::getSRSName() const
    {
        return m_srsName;
    }


    void CityModel::finish(Tesselator& tesselator, bool optimize, bool tesselate, std::shared_ptr<CityGMLLogger> logger)
    {
        // Finish all cityobjcts
        for (auto& cityObj : m_roots) {
            cityObj->finish(tesselator, optimize, tesselate, logger);
        }

        // Build city objects map
        for (std::unique_ptr<CityObject>& obj : m_roots) {
            addToCityObjectsMapRecursive(obj.get());
        }

        // Build id to cityObj map
        for (auto& obj : m_roots) {
            addToIdToCityObjMapRecursive(obj.get());
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
