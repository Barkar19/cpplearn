#ifndef CRULE_H
#define CRULE_H

#include <vector>
#include <ostream>

using namespace std;

class CRule
{

public:
    CRule();


    void SetRule( int attrID, int attrValue, int classID );
    void SetRule( vector<pair<int,int>> rules, int classID );
    void SetClass( int classID );


    vector<pair<int,int>>& GetRules();
    friend std::ostream& operator<<( std::ostream& out, const CRule& r);
private:

    int _classID;
    vector<pair<int,int>> _rules;
};

#endif // CRULE_H
