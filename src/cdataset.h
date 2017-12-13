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

    enum EDiscretizationType
    {
        DISCRETIZATION_INTERVAL = 1,
        DISCRETIZATION_FREQUENCY = 2,
//        DISCRETIZATION_CLUSTERING
    };

    CDataSet();
    CDataSet( const CDataSet& set) = default;
    CDataSet( CDataSet&& set) = default;

    void Load(std::string strFileName, std::string strFormat, char strDelim);
    CDataSet Split( int low, int up ) const;
    CDataSet Cut(int low, int up);
    void Merge( const CDataSet& other );
    std::vector< CDataSet > SplitByClasses() const;

    const std::vector<std::vector<int>>& GetAtributes() const;
    unsigned GetAttributesSize() const;
    unsigned GetSize() const;
    unsigned GetAtributesClassCount( unsigned attrID ) const;
    EAttributeType GetAttributesType( unsigned attrID ) const;
    int ValueToClass( unsigned attrID, int value ) const;

    std::vector<int> AtributesAt( unsigned idx ) const;

    const std::vector<int>& GetTargetValues() const;
    const std::map<int, int> GetTargetMap() const;
    unsigned GetTargetMapSize() const;

    const CDataSet& operator=( CDataSet set);
    friend std::ostream& operator<<( std::ostream& out, const CDataSet& set );

    void DiscretizeAtribute(unsigned attrID, EDiscretizationType type , unsigned bins );
    void Discretize( EDiscretizationType type , unsigned bins );

    const std::vector<double> &GetRealAtribute(unsigned attrID) const;
    double RealValueAt(unsigned idx, unsigned attrID) const;
    std::vector<double> GetRealAtributeValues(unsigned idx) const;

    unsigned CountIf(vector<pair<int, int> > rules) const;
    unsigned Filter(vector<pair<int, int> > rules);
    bool CountIfBool(vector<pair<int, int> > rules) const;
private:

    void Clear();
    void ParseFormat(std::string strFormat);
    int GetDiscreteValue(double value);
    std::string PrintValue(int i, int j) const;

    unsigned int _iSize = 0;

    std::vector< EAttributeType > _aAttributeTypes;

    std::vector< std::vector<int> > _aAttributes;
    std::vector< std::map<int, int> > _aValuesToClassMap;
    std::vector< std::vector<double> > _aRealAttributes;

    std::vector< std::map<int, std::string> > _aCategoricalMaps;


};

#endif // CDATASET_H
