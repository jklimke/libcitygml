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

    const Envelope&CityObject::getEnvelope() const
    {
        return m_envelope;
    }

    unsigned int CityObject::size() const
    {
        return m_geometries.size();
    }

    const Geometry* CityObject::getGeometry(unsigned int i) const
    {
        return m_geometries[i];
    }

    const Composite* CityObject::getComposite(unsigned int i) const
    {
        return m_composites[i];
    }

    unsigned int CityObject::getImplicitGeometryCount() const
    {
        return m_implicitGeometries.size();
    }

    const ImplicitGeometry*CityObject::getImplicitGeometry(unsigned int i) const
    {
        return m_implicitGeometries[i];
    }

    unsigned int CityObject::getChildCount() const
    {
        return m_children.size();
    }

    const CityObject*CityObject::getChild(unsigned int i) const
    {
        return ( i < getChildCount() ) ? m_children[i] : 0;
    }

    CityObject*CityObject::getChild(unsigned int i)
    {
        return ( i < getChildCount() ) ? m_children[i] : 0;
    }

    const std::vector<CityObject*>& CityObject::getChildren() const
    {
        return m_children;
    }

    std::vector<CityObject*>&CityObject::getChildren()
    {
        return m_children;
    }

    void CityObject::finish( AppearanceManager &appearanceManager, const ParserParams& params )
    {

        std::shared_ptr<Appearance> myappearance = appearanceManager.getAppearanceForTarget( getId() );

        std::vector< Geometry* >::const_iterator itGeom = m_geometries.begin();
        for ( ; itGeom != m_geometries.end(); ++itGeom ) {
            if ( !( (*itGeom)->getComposite() ) ) {
                (*itGeom)->finish( appearanceManager, myappearance ? myappearance : 0, params );
            } else {
                Composite* geomComposite = (*itGeom)->getComposite();
                myappearance = appearanceManager.getAppearanceForTarget( geomComposite->getId() );
                (*itGeom)->finish( appearanceManager, myappearance ? myappearance : 0, params );
            }
        }

        std::vector< ImplicitGeometry* >::const_iterator itImplGeom = m_implicitGeometries.begin();
        for( ; itImplGeom != m_implicitGeometries.end(); ++itImplGeom) {
            for(std::vector< Geometry* >::const_iterator it = (*itImplGeom)->_geometries.begin(); it != (*itImplGeom)->_geometries.end(); ++it) {
                if ( !( (*it)->getComposite() ) ) {
                    (*it)->finish( appearanceManager, myappearance ? myappearance : 0, params );
                } else {
                    Composite* geomComposite = (*it)->getComposite();
                    myappearance = appearanceManager.getAppearanceForTarget( geomComposite->getId() );
                    (*it)->finish( appearanceManager, myappearance ? myappearance : 0, params );
                }
            }
        }

        bool finish = false;
        while ( !finish && params.optimize )
        {
            finish = true;
            int len = m_geometries.size();
            for ( int i = 0; finish && i < len - 2; i++ )
            {
                for ( int j = i+1; finish && j < len - 1; j++ )
                {
                    if ( !m_geometries[i]->merge( m_geometries[j] ) ) continue;
                    delete m_geometries[j];
                    m_geometries.erase( m_geometries.begin() + j );
                    finish = false;
                }
            }
        }
    }

    CityObject::~CityObject()
    {
        std::vector< Composite* >::const_iterator itComp = m_composites.begin();
        for ( ; itComp != m_composites.end(); ++itComp )
            delete *itComp;

        std::vector< Geometry* >::const_iterator itGeom = m_geometries.begin();
        for ( ; itGeom != m_geometries.end(); ++itGeom )
            delete *itGeom;

        std::vector< ImplicitGeometry* >::const_iterator itImplGeom = m_implicitGeometries.begin();
        for (; itImplGeom != m_implicitGeometries.end(); ++itImplGeom)
            delete *itImplGeom;
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
