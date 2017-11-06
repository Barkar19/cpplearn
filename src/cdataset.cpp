#include "cdataset.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <limits>

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
    Clear();
    ParseFormat( strFormat );

    _aAttributes = std::vector<std::vector<int>>( _aAttributeTypes.size() );
    _aValuesToClassMap = std::vector<std::map<int,int>>( _aAttributeTypes.size() );
    _aRealAttributes = std::vector<std::vector<double>>( _aAttributeTypes.size() );
    _aCategoricalMaps = std::vector<std::map<int,std::string>>( _aAttributeTypes.size() );

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

CDataSet CDataSet::Split(int low, int up) const
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
    if( !_iSize )
    {
        *this = other;
    }
    else
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

        _aValuesToClassMap = other._aValuesToClassMap;
        _aCategoricalMaps = other._aCategoricalMaps;
    }
}

std::vector<CDataSet> CDataSet::SplitByClasses() const
{
    std::vector< CDataSet > aResult( GetTargetMapSize() );
    for ( unsigned i = 0; i < _iSize; ++i )
    {
        const CDataSet record = this->Split(i,i+1);
        aResult[ ValueToClass( _aAttributes.size() - 1,
                               _aAttributes[ _aAttributes.size() - 1][i] )].Merge( record );
    }
    return aResult;
}

unsigned CDataSet::CountIf( vector<pair<int,int>> rules ) const
{
    unsigned result = 0;
    for ( unsigned i = 0; i < _iSize; ++i )
    {
        bool countRow = true;
        for ( unsigned j = 0; j < rules.size(); ++j )
        {
            const auto& attrID = rules[j].first;
            const auto& attrValue = rules[j].second;
            if ( _aAttributes[attrID][i] != attrValue )
            {
                countRow = false;
                break;
            }
        }
        if ( countRow )
        {
            result++;
        }
    }
    return result;
}

bool CDataSet::CountIfBool( vector<pair<int,int>> rules ) const
{
    for ( unsigned i = 0; i < _iSize; ++i )
    {
        bool countRow = true;
        for ( unsigned j = 0; j < rules.size(); ++j )
        {
            const auto& attrID = rules[j].first;
            const auto& attrValue = rules[j].second;
            if ( _aAttributes[attrID][i] != attrValue )
            {
                countRow = false;
                break;
            }
        }
        if ( countRow )
        {
            return false;
        }
    }
    return true;
}

unsigned CDataSet::Filter(vector<pair<int, int> > rules)
{
    vector<int> idsToRemove;
    for ( unsigned i = 0; i < _iSize; ++i )
    {
        bool countRow = true;
        for ( unsigned j = 0; j < rules.size(); ++j )
        {
            const auto& attrID = rules[j].first;
            const auto& attrValue = rules[j].second;
            if ( _aAttributes[attrID][i] != attrValue )
            {
                countRow = false;
                break;
            }
        }
        if ( countRow )
        {
            idsToRemove.push_back( i );
        }
    }
    for ( auto idx = idsToRemove.rbegin(); idx != idsToRemove.rend(); idx++ )
    {
        for ( unsigned i = 0; i < _aAttributes.size(); ++i )
        {
            _aAttributes[i].erase( _aAttributes[i].begin() + *idx, _aAttributes[i].begin() + *idx+1);
            if ( _aAttributeTypes[i] == ATTR_REAL )
            {
                _aRealAttributes[i].erase( _aRealAttributes[i].begin() + *idx, _aRealAttributes[i].begin() + *idx+1);
            }
        }
        _iSize--;
    }
}

const std::vector<std::vector<int> > &CDataSet::GetAtributes() const
{
    return _aAttributes;
}

const std::vector<double> &CDataSet::GetRealAtribute( unsigned attrID ) const
{
    return _aRealAttributes[attrID];
}

unsigned CDataSet::GetAttributesSize() const
{
    return _aAttributes.size() - 1;
}

unsigned CDataSet::GetSize() const
{
    return _iSize;
}

unsigned CDataSet::GetAtributesClassCount( unsigned attrID ) const
{
    return _aValuesToClassMap[attrID].size();
}

CDataSet::EAttributeType CDataSet::GetAttributesType(unsigned attrID) const
{
    return _aAttributeTypes[attrID];
}

int CDataSet::ValueToClass(unsigned attrID, int value) const
{
    return _aValuesToClassMap[attrID].at(value);
}

std::vector<int> CDataSet::AtributesAt(unsigned idx) const
{
    std::vector<int> result(_aAttributes.size() - 1);
    for ( unsigned i; i < _aAttributes.size() - 1; ++i)
    {
        result[ i ] = _aAttributes[ i ][ idx];
    }
    return result;
}
double CDataSet::RealValueAt( unsigned idx, unsigned attrID ) const
{
    return _aRealAttributes[attrID][ idx ];
}

const std::vector<int> &CDataSet::GetTargetValues() const
{
    return _aAttributes.back();
}

unsigned CDataSet::GetTargetMapSize() const
{
    return _aValuesToClassMap.back().size();
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

void CDataSet::DiscretizeAtribute(unsigned attrID, CDataSet::EDiscretizationType type, unsigned bins)
{
    if ( _aAttributeTypes[ attrID ] == ATTR_REAL &&
         attrID != _aAttributeTypes.size() - 1 )
    {
        const auto& values = _aRealAttributes[attrID];
        std::vector<double> boundaries( bins+1, 0.0 );
        switch ( type )
        {
            case DISCRETIZATION_INTERVAL:
            {
                double minimum = *std::min_element(std::begin(values), std::end(values));
                double maximum = *std::max_element(std::begin(values), std::end(values));
                double interval = ( maximum - minimum ) / bins;
                boundaries[0] = minimum;
                for( unsigned i = 1; i <= bins; ++i )
                {
                    boundaries[i] = minimum + i * interval;
                }
                break;
            }
            case DISCRETIZATION_FREQUENCY:
            {
                unsigned interval = values.size() / bins;
                if( interval )
                {
                    boundaries[0] = *( values.begin() );
                    for( unsigned i = 1; i < bins; ++i )
                    {

                        boundaries[i] = *(values.begin() + i * interval);
                    }
                    boundaries[bins] = values.back();
                }
                else
                {
                    boundaries = values;
                    boundaries.push_back( std::numeric_limits<double>::infinity() );
                }

                break;
            }
//        case DISCRETIZATION_CLUSTERING:
//            {
//                return;
//                break;
//            }
        }
#ifdef DEBUG_DATASET
        std::cout <<"BOUNDARIES FOR ATTR " << attrID << std::endl;
        for ( auto v : boundaries )
        {
            std::cout << v << "\t";
        }std::cout << std::endl;
#endif //DEBUG_DATASET
        for ( unsigned i = 0; i < values.size(); ++i)
        {
            auto num = values[i];
            unsigned idx = 0;
            for( ;idx < boundaries.size()-2 && (boundaries[idx] > num || num > boundaries[idx+1]); idx++ );
            _aAttributes[attrID][i] = idx;
        }
#ifdef DEBUG_BINS
        std::cerr << "BINS " << boundaries.size() - 1 << std::endl;;
        for( unsigned i = 0; i < boundaries.size() - 1; ++i )
        {
            std::cerr << i <<": " << std::count( _aAttributes[attrID].begin(), _aAttributes[attrID].end(), i ) <<std::endl;
        }
        std::cerr << std::endl << std::endl;
#endif

        _aValuesToClassMap[attrID].clear();
        for( unsigned i = 0; i < bins; ++i )
        {

            _aValuesToClassMap[attrID][i] = i;
        }
    }
}

void CDataSet::Discretize(CDataSet::EDiscretizationType type, unsigned bins)
{
    if( bins )
    {
        for( unsigned i = 0; i < _aAttributes.size() - 1; ++i )
        {
            DiscretizeAtribute( i, type, bins );
        }
    }
}

void CDataSet::Clear()
{
    _iSize = 0;

    _aAttributeTypes.clear();
    _aValuesToClassMap.clear();

    _aAttributes.clear();
    _aRealAttributes.clear();
    _aCategoricalMaps.clear();
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
    if ( set._iSize )
    {
        for( unsigned j = 0; j < set._aAttributes.back().size(); ++j )
        {
            out << "[" << j << ".] ROW: ";
            for ( unsigned i =0; i < set._aAttributes.size(); ++i )
            {
                out << std::left <<std::setw(20) << set.PrintValue(i, j);
            }
            out << std::endl;
        }
    }
    return out;
}
