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
//    cout << "COPY!";
//}

void CDataSet::ParseFormat( string strFormat )
{
    stringstream stream( strFormat );
    string buf;
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
            cerr << "Wrong format string! Exiting...\n";
            exit(1);
        }
    }
}

void CDataSet::Load( string strFileName, string strFormat, char strDelim )
{
    ParseFormat( strFormat );

    _aAttributes = vector<vector<int>>( _aAttributeTypes.size() );
    _aRealAttributes = vector<vector<double>>( _aAttributeTypes.size() );
    _aCategoricalMaps = vector<map<int,string>>( _aAttributeTypes.size() );

//    for( const EAttributeType& t : _aAttributeTypes )
//    {
//        _aAttributes.push_back( vector<int>() );
//        _aRealAttributes.push_back( vector<double>() );
//        _aCategoricalMaps.push_back( map<int,string>() );
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

    ifstream file( strFileName );

    for ( string line; getline( file, line); )
    {
        istringstream stream( line );
//        int integerAttrId = 0, realAttrId = 0, categoricalAttrId = 0;
        int currentAttrId = 0;
        for( const EAttributeType& t : _aAttributeTypes )
        {
            string buf;
            getline( stream, buf, strDelim );
            if ( buf == "" )
            {
                break;
            }
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
                                      [buf](const std::pair<int, string> & t) -> bool
                                            {
                                                return t.second == buf;
                                            }
                                    );
                    if( it == _aCategoricalMaps[ currentAttrId ].end() )
                    {
                        value = _aCategoricalMaps[ currentAttrId ].size();
                        _aCategoricalMaps[ currentAttrId ][ value ] = buf;
                        cout << "KEY: " << value << " VAL: "<< buf << endl;
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

    result._aAttributes = vector< vector<int> >( _aAttributes.size() );
    result._aRealAttributes = vector<vector<double>>( _aAttributeTypes.size() );
    result._aCategoricalMaps = _aCategoricalMaps;
    for ( unsigned i = 0; i < _aAttributes.size(); ++i )
    {
        result._aAttributes[i] = vector<int>( &(_aAttributes[i][low]), &(_aAttributes[i][up]) );
        if ( _aAttributeTypes[i] == ATTR_REAL )
        {
            result._aRealAttributes[i] = vector<double>( &(_aRealAttributes[i][low]), &(_aRealAttributes[i][up]) );
        }
    }

    result._iSize = result._aAttributes.back().size();
    return result;
}

CDataSet CDataSet::Cut(int low, int up)
{
    CDataSet result;

    result._aAttributeTypes = _aAttributeTypes;

    result._aAttributes = vector< vector<int> >( _aAttributes.size() );
    result._aRealAttributes = vector<vector<double>>( _aAttributeTypes.size() );
    result._aCategoricalMaps = _aCategoricalMaps;
    for ( unsigned i = 0; i < _aAttributes.size(); ++i )
    {
        result._aAttributes[i] = vector<int>( &(_aAttributes[i][low]), &(_aAttributes[i][up]) );
        _aAttributes[i].erase( _aAttributes[i].begin() + low, _aAttributes[i].begin() + up);
        if ( _aAttributeTypes[i] == ATTR_REAL )
        {
            result._aRealAttributes[i] = vector<double>( &(_aRealAttributes[i][low]), &(_aRealAttributes[i][up]) );
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

int CDataSet::GetSize() const
{
    return _iSize;
}

const vector<int> &CDataSet::GetTargetValues()
{
    return _aAttributes.back();
}

const CDataSet &CDataSet::operator=( CDataSet set )
{
    std::swap( _iSize, set._iSize );

    std::swap( _aAttributeTypes, set._aAttributeTypes );

    std::swap( _aAttributes, set._aAttributes );
    std::swap( _aRealAttributes, set._aRealAttributes );
    std::swap( _aCategoricalMaps, set._aCategoricalMaps );

    return *this;
}

string CDataSet::PrintValue( int i, int j ) const
{
    string result;
    switch( static_cast<CDataSet::EAttributeType>( _aAttributeTypes[i] ) )
    {
        case CDataSet::ATTR_INT:
        {
            result = to_string( _aAttributes[i][j] );
            break;
        }
        case CDataSet::ATTR_REAL:
        {
            result = to_string( _aAttributes[i][j ] ) + string(" ( ") + to_string(_aRealAttributes[i][j]) + string(" )");
            break;
        }
        case CDataSet::ATTR_STRING:
        {
            result = to_string( _aAttributes[i][j ] ) + string(" ( ") + _aCategoricalMaps[i].at( _aAttributes[i][j] )  + string(" )");
            break;
        }
    }
    return result;
}

ostream& operator<<( ostream &out, const CDataSet &set )
{
    out << "DATA SET SIZE "<< set._iSize << "\n";
    out << "ATTRSIBUTES: " << set._aAttributes.size() << endl;


    for( unsigned j = 0; j < set._aAttributes.back().size(); ++j )
    {
        out << "[" << j << ".] ROW: ";
        for ( unsigned i =0; i < set._aAttributes.size(); ++i )
        {
            out << set.PrintValue(i, j) << "\t";
        }
        out << endl;
    }
//    for( unsigned i = 0; i < set._aAttributes.size(); ++i )
//    {
//        const auto& column = set._aAttributes[i];
//        out << static_cast<CDataSet::EAttributeType>( set._aAttributeTypes[i] )<<" ATTR " << i << " SIZE: "<< column.size()  << endl;
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
//            out << endl;
//        }
//    }
    return out;
}
