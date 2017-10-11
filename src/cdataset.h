#ifndef CDATASET_H
#define CDATASET_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class CDataSet
{
public:
    enum EAttributeType
    {
        ATTR_INT,
        ATTR_REAL,
        ATTR_STRING
    };

    CDataSet();
    CDataSet( const CDataSet& set) = default;
    CDataSet( CDataSet&& set) = default;

    void Load(string strFileName, string strFormat, char strDelim);
    CDataSet Split( int low, int up );
    CDataSet Cut( int low, int up );
    void Merge( const CDataSet& other );

    int GetSize() const;
    const vector<int>& GetTargetValues();

    const CDataSet& operator=( CDataSet set);
    friend ostream& operator<<( ostream& out, const CDataSet& set );

private:

    void ParseFormat(string strFormat);
    int GetDiscreteValue(double value);
    string PrintValue(int i, int j) const;

    unsigned int _iSize = 0;

    vector< EAttributeType > _aAttributeTypes;

    vector< vector<int> > _aAttributes;
    vector< vector<double> > _aRealAttributes;

    vector< map<int, string> > _aCategoricalMaps;


};

#endif // CDATASET_H
