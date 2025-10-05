#pragma once
#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string.h>
#include <string_view>
#include <utility>

// Utils

template <typename T>
std::pair<T, char const*> readNextNumber(std::string_view const& string) {
    char const* const firstPattern = std::find_if(string.data(), string.data() + string.size(), [](char ch) {
        return !std::isspace(ch);
    });
    if (firstPattern == string.data() + string.size()) {
        throw std::runtime_error("Cannot parse number.");
    }

    T result;
    auto [patternEnd, errorCode] = std::from_chars(firstPattern, string.data() + string.size(), result);
    if (errorCode == std::errc()) {
        char const* const nextPattern = std::find_if(patternEnd, string.data() + string.size(), [](char ch) {
            return !std::isspace(ch);
        });
        bool const nonWhitespaceAfterNumber = patternEnd != string.data() + string.size() && nextPattern == patternEnd;
        if (nonWhitespaceAfterNumber) {
            throw std::runtime_error("Cannot parse number.");
        }
        return { result, nextPattern };
    } else {
        throw std::runtime_error("Cannot parse number.");
    }
}

template <typename T, size_t N>
char const* readNextNumbers(std::string_view view, std::array<T*, N> const& targets) {
    for (T* target : targets) {
        if (view.empty()) {
            throw std::runtime_error("Cannot parse number from empty string.");
        } else {
            char const* end;
            std::tie(*target, end) = readNextNumber<T>(view);
            view = view.substr(std::distance(view.data(), end));
        }
    }
    return view.data();
}

// 2D vector class.

template< class T > class TVec2
{
public:
    T x;
    T y;

public:
    TVec2( const TVec2<T>& o) {
        this->x = o.x;
        this->y = o.y;
    }

    TVec2( const T x = (T)0, const T y = (T)0 );
    TVec2( const T vec[] );

    char const* fromString(std::string_view view);

    TVec2 operator+( const TVec2<T>& v ) const;
    TVec2 operator-( const TVec2<T>& v ) const;
    TVec2 operator-() const;
    TVec2 operator*( const T& v ) const;
    TVec2 operator*( const TVec2<T>& v ) const;
    TVec2 operator/( const T& v ) const;
    TVec2 operator/( const TVec2<T>& v ) const;

    bool operator==( const TVec2<T>& v ) const;
    bool operator!=( const TVec2<T>& v ) const;
};

template< class T > inline TVec2<T>::TVec2( const T x, const T y )
{
    this->x = x;
    this->y = y;
}

template< class T >	inline TVec2<T>::TVec2( const T vec[] )
{
    x = vec[0];
    y = vec[1];
}

template< class T >	inline TVec2<T> TVec2<T>::operator+( const TVec2<T>& v ) const
{
    return TVec2<T>( x + v.x, y + v.y );
}

template< class T >	inline TVec2<T> TVec2<T>::operator-( const TVec2<T>& v ) const
{
    return TVec2<T>( x - v.x, y - v.y );
}

template< class T >	inline TVec2<T> TVec2<T>::operator-() const
{
    return TVec2<T>( -x, -y );
}

template< class T >	inline TVec2<T> TVec2<T>::operator*( const T& v ) const
{
    return TVec2<T>( x * v, y * v );
}

template< class T >	inline TVec2<T> TVec2<T>::operator*( const TVec2<T>& v ) const
{
    return TVec2<T>( x * v.x, y * v.y );
}

template< class T >	inline TVec2<T> TVec2<T>::operator/( const T& v ) const
{
    return TVec2<T>( x / v, y / v );
}

template< class T >	inline TVec2<T> TVec2<T>::operator/( const TVec2<T>& v ) const
{
    return TVec2<T>( x / v.x, y / v.y );
}

template< class T >	inline bool TVec2<T>::operator==( const TVec2<T>& v ) const
{
    return x == v.x && y == v.y;
}

template< class T >	inline bool TVec2<T>::operator!=( const TVec2<T>& v ) const
{
    return x != v.x || y != v.y;
}

template< class T >	inline TVec2<T> operator*( const T& val, const TVec2<T>& vec )
{
    return TVec2<T>( vec.x * val, vec.y * val );
}

template<class T> inline std::ostream& operator<<(std::ostream & os, TVec2<T> const & v)
{
    return os << std::fixed << v.x << " " << std::fixed << v.y;
}

template<class T> char const* TVec2<T>::fromString(std::string_view view) {
    return readNextNumbers(view, std::array<T*, 2>{ &x, &y });
}

typedef TVec2< float >			TVec2f;
typedef TVec2< double >			TVec2d;


// 3D vector class.

template< class T > class TVec3
{
public:
    T x, y, z;

public:
    TVec3( const T x = (T)0, const T y = (T)0, const T z = (T)0 );

    inline T length() const;
    inline T sqrLength() const;

    char const* fromString(std::string_view view);

    T dot( const TVec3<T>& vec ) const;
    TVec3 cross( const TVec3<T>& vec ) const;

    TVec3  normal() const;
    TVec3& normalEq();
    TVec3& normalEq( const T length );

    TVec3 operator+( const TVec3<T>& rhs ) const;
    TVec3 operator+( const T& _v ) const;
    TVec3 operator-( const TVec3<T>& rhs ) const;
    TVec3 operator-( const T& _v ) const;
    TVec3 operator-() const;
    TVec3 operator*( const T& rhs ) const;
    TVec3 operator*( const TVec3<T>& rhs ) const;
    TVec3 operator/( const T& rhs ) const;
    TVec3 operator/( const TVec3<T>& rhs ) const;

    inline bool operator==( const TVec3<T>& rhs ) const;
    inline bool operator!=( const TVec3<T>& rhs ) const;
};

template< class T > inline TVec3<T>::TVec3( const T x, const T y, const T z )
{
    this->x = x;
    this->y = y;
    this->z = z;
}

template< class T > inline T TVec3<T>::length() const
{
    return (T)sqrt( x*x + y*y + z*z );
}

template< class T > inline T TVec3<T>::sqrLength() const
{
    return x*x + y*y + z*z;
}

template< class T > inline T TVec3<T>::dot( const TVec3<T>& vec ) const
{
    return x*vec.x + y*vec.y + z*vec.z;
}

template< class T > inline TVec3<T> TVec3<T>::cross( const TVec3<T>& vec ) const
{
    return TVec3<T>( y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x );
}

template< class T > inline TVec3<T> TVec3<T>::normal() const
{
    const T len = length();
    const T tmp = ( len != (T)0 ) ? ( (T)1 / len ) : (T)0;
    return TVec3<T>( x * tmp, y * tmp, z * tmp );
}

template< class T > inline TVec3<T>& TVec3<T>::normalEq()
{
    const T len = length();
    const T tmp = ( len != (T)0 ) ? ( (T)1 / len ) : (T)0;
    x *= tmp;
    y *= tmp;
    z *= tmp;
    return *this;
}

template< class T > inline TVec3<T>& TVec3<T>::normalEq( const T length )
{
    const T len = TVec3<T>::length();
    const T tmp = ( len != (T)0 ) ? length / len : (T)0;
    x *= tmp;
    y *= tmp;
    z *= tmp;
    return *this;
}

template< class T > inline TVec3<T> TVec3<T>::operator+( const TVec3<T>& rhs ) const
{
    return TVec3<T>( x + rhs.x, y + rhs.y, z + rhs.z );
}

template< class T >	inline TVec3<T> TVec3<T>::operator+( const T& _v ) const
{
    return TVec3<T>( x + _v, y + _v, z + _v );
}

template< class T > inline TVec3<T> TVec3<T>::operator-( const TVec3<T>& rhs ) const
{
    return TVec3<T>( x - rhs.x, y - rhs.y, z - rhs.z );
}

template< class T > inline TVec3<T> TVec3<T>::operator-( const T& _v ) const
{
    return TVec3<T>( x - _v, y - _v, z - _v );
}

template< class T >	inline TVec3<T> TVec3<T>::operator-() const
{
    return TVec3<T>( -x, -y, -z );
}

template< class T > inline TVec3<T> TVec3<T>::operator*( const T& rhs ) const
{
    return TVec3<T>( x * rhs, y * rhs, z * rhs );
}

template< class T >	inline TVec3<T> TVec3<T>::operator*( const TVec3<T>& rhs ) const
{
    return TVec3<T>( x * rhs.x, y * rhs.y, z * rhs.z );
}

template< class T > inline TVec3<T> TVec3<T>::operator/( const T& rhs ) const
{
    return TVec3<T>( x / rhs, y / rhs, z / rhs );
}

template< class T > inline TVec3<T> TVec3<T>::operator/( const TVec3<T>& rhs ) const
{
    return TVec3<T>( x / rhs.x, y / rhs.y, z / rhs.z );
}

template< class T > inline bool TVec3<T>::operator==( const TVec3<T>& rhs ) const
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

template< class T > inline bool TVec3<T>::operator!=( const TVec3<T>& rhs ) const
{
    return x != rhs.x || y != rhs.y || z != rhs.z;
}

template< class T >	inline TVec3<T> operator*( const T& val, const TVec3<T>& vec )
{
    return TVec3<T>( vec.x * val, vec.y * val, vec.z * val );
}

template<class T> inline std::ostream& operator<<(std::ostream & os, const TVec3<T> & v)
{
    return os << std::fixed << v.x << " " << std::fixed << v.y << " " << std::fixed << v.z;
}

template<class T> char const* TVec3<T>::fromString(std::string_view view) {
    return readNextNumbers(view, std::array<T*, 3>{ &x, &y, &z });
}

typedef TVec3< float >			TVec3f;
typedef TVec3< double >			TVec3d;


// 4D vector class.

template< class T > class TVec4
{
public:
    T x, y, z, w;

public:
    TVec4( const T x = (T)0, const T y = (T)0, const T z = (T)0, const T w = (T)0 )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    char const* fromString(std::string_view view);
};

template<class T> inline std::ostream& operator<<( std::ostream & os, TVec4<T> const & v )
{
    return os << std::fixed << v.x << " " << std::fixed << v.y << " " << std::fixed << v.z << " " << std::fixed << v.w;
}

template<class T> char const* TVec4<T>::fromString(std::string_view view) {
    return readNextNumbers(view, std::array<T*, 4>{ &x, &y, &z, &w });
}

typedef TVec4< float >			TVec4f;
typedef TVec4< double >			TVec4d;

