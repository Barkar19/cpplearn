#include "cdataset.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

CDataSet::CDataSet()
{

}

//CDataSet::CDataSet(const CDataSet &set)
//{
//    std::cout << "COPY!";
//}

void CDataSet::ParseFormat( std::string strFormat )
{
    std::stringstream stream( strFormat );
    std::string buf;
    while ( stream >> buf )
    {
        if ( buf == "%d" )
        {
            _aAttributeTypes.push_back( ATTR_INT );
        }
        else if ( buf == "%f")
        {
            _aAttributeTypes.push_back( ATTR_REAL );
        }
        else if ( buf == "%s" )
        {
            _aAttributeTypes.push_back( ATTR_STRING );
        }
        else
        {
            std::cerr << "Wrong format std::string! Exiting...\n";
            exit(1);
        }
    }
}

void CDataSet::Load( std::string strFileName, std::string strFormat, char strDelim )
{
    ParseFormat( strFormat );

    _aAttributes = std::vector<std::vector<int>>( _aAttributeTypes.size() );
    _aValuesToClassMap = std::vector<std::map<int,int>>( _aAttributeTypes.size() );
    _aRealAttributes = std::vector<std::vector<double>>( _aAttributeTypes.size() );
    _aCategoricalMaps = std::vector<std::map<int,std::string>>( _aAttributeTypes.size() );

//    for( const EAttributeType& t : _aAttributeTypes )
//    {
//        _aAttributes.push_back( std::vector<int>() );
//        _aRealAttributes.push_back( std::vector<double>() );
//        _aCategoricalMaps.push_back( std::map<int,std::string>() );
////        switch( t )
////        {
////            case ATTR_INT:
////            {
////                break;
////            }
////            case ATTR_REAL:
////            {
////                break;
////            }
////            case ATTR_STRING:
////            {
////                break;
////            }
////        }
//    }

    std::ifstream file( strFileName );

    std::vector<std::string> lines;
    for ( std::string line; std::getline( file, line); )
    {
        if( line.find_first_not_of("\t\n ") != std::string::npos )
        {
#ifdef DEBUG_DATASET
            std::cout << "<line>" << line <<" </line>\n";
#endif
            lines.push_back( line );
        }
    }

    std::random_shuffle( lines.begin(), lines.end() );

    for ( const auto& line : lines )
    {
        std::stringstream stream( line );
//        int integerAttrId = 0, realAttrId = 0, categoricalAttrId = 0;
        int currentAttrId = 0;
        for( const EAttributeType& t : _aAttributeTypes )
        {
            std::string buf;
            getline( stream, buf, strDelim );
            int value = 0;
            switch( t )
            {
                case ATTR_INT:
                {
                    value = stol( buf );
                    break;
                }
                case ATTR_REAL:
                {
                    double realValue = stod( buf );
                    _aRealAttributes[ currentAttrId ].push_back( realValue );
//                    currentAttrId = ++realAttrId % _aRealAttributes.size();

                    value = GetDiscreteValue( realValue );
                    break;
                }
                case ATTR_STRING:
                {
                    auto it = find_if(_aCategoricalMaps[ currentAttrId ].begin(),
                                      _aCategoricalMaps[ currentAttrId ].end(),
                                      [buf](const std::pair<int, std::string> & t) -> bool
                                            {
                                                return t.second == buf;
                                            }
                                    );
                    if( it == _aCategoricalMaps[ currentAttrId ].end() )
                    {
                        value = _aCategoricalMaps[ currentAttrId ].size();
                        _aCategoricalMaps[ currentAttrId ][ value ] = buf;
#ifdef DEBUG_DATASET
                        std::cout << "KEY: " << value << " VAL: "<< buf << std::endl;
#endif
                    }
                    else
                    {
                        value = it->first;
                    }
//                    categoricalAttrId = ++categoricalAttrId % _aCategoricalMaps.size();
                    break;
                }
            }
            _aAttributes[ currentAttrId ].push_back( value );

            if ( _aValuesToClassMap[ currentAttrId ].find( value ) == _aValuesToClassMap[ currentAttrId ].end() )
            {
                _aValuesToClassMap[ currentAttrId ][ value ] = _aValuesToClassMap[ currentAttrId ].size();
            }

            currentAttrId = ( currentAttrId + 1 ) % _aAttributes.size();
        }
    }

    // TO DO Class labels not in last categorical
    _iSize = _aAttributes.back().size();

}

int CDataSet::GetDiscreteValue( double value )
{
    return value;
}

CDataSet CDataSet::Split(int low, int up)
{
    CDataSet result;

    result._aAttributeTypes = _aAttributeTypes;
    result._aValuesToClassMap = _aValuesToClassMap;

    result._aAttributes = std::vector< std::vector<int> >( _aAttributes.size() );
    result._aRealAttributes = std::vector<std::vector<double>>( _aAttributeTypes.size() );
    result._aCategoricalMaps = _aCategoricalMaps;
    for ( unsigned i = 0; i < _aAttributes.size(); ++i )
    {
        result._aAttributes[i] = std::vector<int>( &(_aAttributes[i][low]), &(_aAttributes[i][up]) );
        if ( _aAttributeTypes[i] == ATTR_REAL )
        {
            result._aRealAttributes[i] = std::vector<double>( &(_aRealAttributes[i][low]), &(_aRealAttributes[i][up]) );
        }
    }

    result._iSize = result._aAttributes.back().size();
    return result;
}

CDataSet CDataSet::Cut(int low, int up)
{
    CDataSet result;

    result._aAttributeTypes = _aAttributeTypes;
    result._aValuesToClassMap = _aValuesToClassMap;

    result._aAttributes = std::vector< std::vector<int> >( _aAttributes.size() );
    result._aRealAttributes = std::vector<std::vector<double>>( _aAttributeTypes.size() );
    result._aCategoricalMaps = _aCategoricalMaps;
    for ( unsigned i = 0; i < _aAttributes.size(); ++i )
    {
        result._aAttributes[i] = std::vector<int>( &(_aAttributes[i][low]), &(_aAttributes[i][up]) );
        _aAttributes[i].erase( _aAttributes[i].begin() + low, _aAttributes[i].begin() + up);
        if ( _aAttributeTypes[i] == ATTR_REAL )
        {
            result._aRealAttributes[i] = std::vector<double>( &(_aRealAttributes[i][low]), &(_aRealAttributes[i][up]) );
            _aRealAttributes[i].erase( _aRealAttributes[i].begin() + low, _aRealAttributes[i].begin() + up);
        }
    }
    _iSize = _aAttributes.back().size();
    result._iSize = result._aAttributes.back().size();
    return result;
}

void CDataSet::Merge(const CDataSet &other )
{
    if ( _aAttributeTypes == other._aAttributeTypes )
    {
        for ( unsigned i = 0; i < _aAttributes.size(); ++i )
        {
            _aAttributes[i].reserve( _aAttributes[i].size() + other._aAttributes[i].size() );
            _aAttributes[i].insert( _aAttributes[i].end(), other._aAttributes[i].begin(), other._aAttributes[i].end() );
            if ( _aAttributeTypes[i] == ATTR_REAL )
            {
                _aRealAttributes[i].reserve( _aRealAttributes[i].size() + other._aRealAttributes[i].size() );
                _aRealAttributes[i].insert( _aRealAttributes[i].end(), other._aRealAttributes[i].begin(), other._aRealAttributes[i].end() );
            }
        }
    }
    _iSize = _aAttributes.back().size();
}

const std::vector<std::vector<int> > &CDataSet::GetAtributes() const
{
    return _aAttributes;
}

int CDataSet::GetAttributesSize() const
{
    return _aAttributes.size() - 1;
}

int CDataSet::GetSize() const
{
    return _iSize;
}

int CDataSet::GetAtributesClassCount( unsigned attrID ) const
{
    return _aValuesToClassMap[attrID].size();
}

int CDataSet::ValueToClass(unsigned attrID, int value) const
{
    return _aValuesToClassMap[attrID].at(value);
}

const std::vector<int> &CDataSet::GetTargetValues() const
{
    return _aAttributes.back();
}

const std::map<int, std::string> CDataSet::GetTargetMap() const
{
    return _aCategoricalMaps.back();
}

const CDataSet &CDataSet::operator=( CDataSet set )
{
    std::swap( _iSize, set._iSize );

    std::swap( _aAttributeTypes, set._aAttributeTypes );
    std::swap( _aValuesToClassMap, set._aValuesToClassMap );

    std::swap( _aAttributes, set._aAttributes );
    std::swap( _aRealAttributes, set._aRealAttributes );
    std::swap( _aCategoricalMaps, set._aCategoricalMaps );

    return *this;
}

std::string CDataSet::PrintValue( int i, int j ) const
{
    std::string result;
    switch( static_cast<CDataSet::EAttributeType>( _aAttributeTypes[i] ) )
    {
        case CDataSet::ATTR_INT:
        {
            result = std::to_string( _aAttributes[i][j] );
            break;
        }
        case CDataSet::ATTR_REAL:
        {
            result = std::to_string( _aAttributes[i][j ] ) + std::string(" ( ") + std::to_string(_aRealAttributes[i][j]) + std::string(" )");
            break;
        }
        case CDataSet::ATTR_STRING:
        {
            result = std::to_string( _aAttributes[i][j ] ) + std::string(" ( ") + _aCategoricalMaps[i].at( _aAttributes[i][j] )  + std::string(" )");
            break;
        }
    }
    return result;
}

std::ostream& operator<<( std::ostream &out, const CDataSet &set )
{
    out << "DATA SET SIZE "<< set._iSize << "\n";
    out << "ATTRSIBUTES: " << set._aAttributes.size() << std::endl;


    for( unsigned j = 0; j < set._aAttributes.back().size(); ++j )
    {
        out << "[" << j << ".] ROW: ";
        for ( unsigned i =0; i < set._aAttributes.size(); ++i )
        {
            out << set.PrintValue(i, j) << "\t";
        }
        out << std::endl;
    }
//    for( unsigned i = 0; i < set._aAttributes.size(); ++i )
//    {
//        const auto& column = set._aAttributes[i];
//        out << static_cast<CDataSet::EAttributeType>( set._aAttributeTypes[i] )<<" ATTR " << i << " SIZE: "<< column.size()  << std::endl;
//        for ( unsigned j = 0; j < column.size(); ++j )
//        {
//            out << column[j];
//            switch( static_cast<CDataSet::EAttributeType>( set._aAttributeTypes[i] ) )
//            {
//                case CDataSet::ATTR_INT:
//                {
//                    break;
//                }
//                case CDataSet::ATTR_REAL:
//                {
//                    out << " ( " << set._aRealAttributes[i][j] << " )";

//                    break;
//                }
//                case CDataSet::ATTR_STRING:
//                {
//                    out << " ( " << set._aCategoricalMaps[i].at( column[j] ) << " )";
//                    break;
//                }
//            }
//            out << std::endl;
//        }
//    }
    return out;
}
