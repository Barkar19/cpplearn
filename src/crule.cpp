#include "crule.h"
#include <ostream>

CRule::CRule()
{

}

void CRule::SetRule(int attrID, int attrValue, int classID)
{
    pair<int,int> p( attrID, attrValue);
    _rules.push_back( p );
    _classID = classID;
}

void CRule::SetRule(vector<pair<int, int> > rules, int classID)
{
    _rules = rules;
    _classID = classID;
}

vector<pair<int, int> > &CRule::GetRules()
{
    return _rules;
}

std::ostream& operator<<( std::ostream& out, const CRule& r)
{
    out << "IF ";
    for ( unsigned i = 0; i < r._rules.size(); ++i )
    {
        out << "CLASS " << r._rules[i].first << " == " << r._rules[i].second << "\n";
    }
    out << "THEN " << r._classID << "\n";
    return out;
}
