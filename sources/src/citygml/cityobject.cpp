#include "citygml/cityobject.h"
#include "citygml/geometry.h"
#include "citygml/implictgeometry.h"
#include "citygml/composite.h"
#include "citygml/appearancemanager.h"
#include "citygml/citygml.h"

#include <sstream>

#define GETCITYNAME( _t_ ) if ( mask & COT_ ## _t_ ) ss << # _t_ << "|";

#define COMPARECITYNAMEMASK( _t_ ) {\
        bool neg = ( tokens[i][0] == '~' || tokens[i][0] == '!' );\
        if ( ci_string_compare( #_t_, neg ? tokens[i].substr(1) : tokens[i] ) ) { mask = neg ? ( mask & (~ COT_ ## _t_ )) : ( mask | COT_ ## _t_ );}\
    }

namespace citygml {

    CityObject::CityObject(const std::string& id, CityObject::CityObjectsType type)  : Object( id ), m_type( type )
    {

    }

    CityObject::CityObjectsType CityObject::getType() const
    {
        return m_type;
    }

    std::string CityObject::getTypeAsString() const
    {

        std::stringstream ss;
        GETCITYNAME( GenericCityObject );
        GETCITYNAME( Building );
        GETCITYNAME( BuildingPart );
        GETCITYNAME( Room );
        GETCITYNAME( BuildingInstallation );
        GETCITYNAME( BuildingFurniture );
        GETCITYNAME( Door );
        GETCITYNAME( Window );
        GETCITYNAME( CityFurniture );
        GETCITYNAME( Track );
        GETCITYNAME( Road );
        GETCITYNAME( Railway );
        GETCITYNAME( Square );
        GETCITYNAME( PlantCover );
        GETCITYNAME( SolitaryVegetationObject );
        GETCITYNAME( WaterBody );
        GETCITYNAME( TINRelief );
        GETCITYNAME( LandUse );
        GETCITYNAME( Tunnel );
        GETCITYNAME( Bridge );
        GETCITYNAME( BridgeConstructionElement );
        GETCITYNAME( BridgeInstallation );
        GETCITYNAME( BridgePart );
        GETCITYNAME( RoofSurface );
        GETCITYNAME( WallSurface );
        GETCITYNAME( GroundSurface );
        GETCITYNAME( ClosureSurface );
        GETCITYNAME( FloorSurface );
        GETCITYNAME( InteriorWallSurface );
        GETCITYNAME( CeilingSurface );
        std::string s = ss.str();
        if ( s != "" ) s.erase( s.length() - 1, 1 ); // remove the last | char
        return s;
    }

    const Envelope& CityObject::getEnvelope() const
    {
        return m_envelope;
    }

    void CityObject::setEnvelope(Envelope envelope)
    {
        m_envelope = envelope;
    }

    unsigned int CityObject::getGeometriesCount() const
    {
        return m_geometries.size();
    }

    const Geometry& CityObject::getGeometry(unsigned int i) const
    {
        return *m_geometries[i];
    }

    unsigned int CityObject::getImplicitGeometryCount() const
    {
        return m_implicitGeometries.size();
    }

    const ImplicitGeometry& CityObject::getImplicitGeometry(unsigned int i) const
    {
        return *m_implicitGeometries[i];
    }

    unsigned int CityObject::getChildCityObjecsCount() const
    {
        return m_children.size();
    }

    const CityObject& CityObject::getChildCityObject(unsigned int i) const
    {
        return *m_children[i];
    }

    CityObject& CityObject::getChildCityObject(unsigned int i)
    {
        return *m_children[i];
    }

    void CityObject::finish(bool tesselate, Tesselator& tesselator , bool mergeGeometries)
    {

        if (mergeGeometries) {

            // Try merge Geometries pairwise from left to right
            int mergeTargetIndex = 0;
            while (mergeTargetIndex < m_geometries.size()) {

                int mergeSourceIndex = mergeTargetIndex + 1;

                while (mergeSourceIndex < m_geometries.size()) {

                    if (m_geometries[mergeTargetIndex]->merge(m_geometries[mergeSourceIndex])) {
                        m_geometries.erase(m_geometries.begin() + mergeSourceIndex);
                    } else {
                        mergeSourceIndex++;
                    }

                }

                mergeTargetIndex++;

            }

        }

        for (std::unique_ptr<Geometry> geom : m_geometries) {
            geom->addAppearancesOf(*this);
            geom->finish(tesselate, tesselator, mergeGeometries);
        }

        for (std::unique_ptr<ImplicitGeometry> implictGeom : m_implicitGeometries) {
            for (int i = 0; i < implictGeom->getGeometriesCount(); i++) {
                implictGeom[i]->addAppearancesOf(*this);
                implictGeom[i]->finish(tesselate, tesselator, mergeGeometries);
            }
        }

        for (std::unique_ptr<CityObject> child : m_children) {
            child->addAppearancesOf(*this);
            child->finish(tesselate, tesselator, mergeGeometries);
        }
    }

    CityObject::~CityObject()
    {
    }

    std::ostream& operator<<( std::ostream& os, const CityObject& o )
    {
        os << o.getType() << ": " << o.getId() << std::endl;
        os << "  Envelope: " << o.getEnvelope() << std::endl;

        AttributesMap::const_iterator it = o.m_attributes.begin();
        while ( it != o.m_attributes.end() )
        {
            os << "  + " << it->first << ": " << it->second << std::endl;
            it++;
        }

        std::vector< Geometry* >::const_iterator itp = o._geometries.begin();
        for ( ; itp != o._geometries.end(); itp++ )
            os << **itp;

        os << "  * " << o._geometries.size() << " geometries." << std::endl;

        return os;
    }

    CityObjectsTypeMask getCityObjectsTypeMaskFromString( const std::string& stringMask )
    {
        CityObjectsTypeMask mask = 0;

        std::vector<std::string> tokens = tokenize( stringMask );

        for ( unsigned int i = 0; i < tokens.size(); i++ )
        {
            if ( tokens[i].length() == 0 ) continue;

            COMPARECITYNAMEMASK( GenericCityObject );
            COMPARECITYNAMEMASK( Building );
            COMPARECITYNAMEMASK( BuildingPart );
            COMPARECITYNAMEMASK( Room );
            COMPARECITYNAMEMASK( BuildingInstallation );
            COMPARECITYNAMEMASK( BuildingFurniture );
            COMPARECITYNAMEMASK( Door );
            COMPARECITYNAMEMASK( Window );
            COMPARECITYNAMEMASK( CityFurniture );
            COMPARECITYNAMEMASK( Track );
            COMPARECITYNAMEMASK( Road );
            COMPARECITYNAMEMASK( Railway );
            COMPARECITYNAMEMASK( Square	);
            COMPARECITYNAMEMASK( PlantCover	);
            COMPARECITYNAMEMASK( SolitaryVegetationObject );
            COMPARECITYNAMEMASK( WaterBody );
            COMPARECITYNAMEMASK( TINRelief );
            COMPARECITYNAMEMASK( LandUse );
            COMPARECITYNAMEMASK( Tunnel );
            COMPARECITYNAMEMASK( Bridge );
            COMPARECITYNAMEMASK( BridgeConstructionElement );
            COMPARECITYNAMEMASK( BridgeInstallation );
            COMPARECITYNAMEMASK( BridgePart );
            COMPARECITYNAMEMASK( GenericCityObject );

            COMPARECITYNAMEMASK( WallSurface );
            COMPARECITYNAMEMASK( RoofSurface );
            COMPARECITYNAMEMASK( GroundSurface );
            COMPARECITYNAMEMASK( ClosureSurface );
            COMPARECITYNAMEMASK( FloorSurface );
            COMPARECITYNAMEMASK( InteriorWallSurface );
            COMPARECITYNAMEMASK( CeilingSurface );
            COMPARECITYNAMEMASK( All );
        }
        return mask;
    }

}
