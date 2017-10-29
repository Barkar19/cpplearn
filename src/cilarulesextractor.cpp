#include "cilarulesextractor.h"
#include <algorithm>
#include <iostream>
#include "crule.h"

CILARulesExtractor::CILARulesExtractor()
{

}

vector<CRule> CILARulesExtractor::ExtractRules(const CDataSet &data)
{

//    for( ... ) // FOR EACH SUBSET
//    {
//        // SET OF ATTRIBUTES ( K OF N, for K = 1 ... N)
//        // MAX ROWS PER RULE = 0
        unsigned attributesCount = 0;
//        do
//        {
            attributesCount += 1;
            std::string bitmask( attributesCount, 1);
            bitmask.resize( data.GetAttributesSize(), 0);
            do
            {
                CRule currentRuleAttr = GetRuleTemplate( bitmask );
                do
                {
                    cout << currentRuleAttr;
//                    CHECK UNIQUE IN REST SUBSETS

//                    COUNT ROWS

//                    UPDATE MAX_ROWS
                } while ( GetNextRule(data, currentRuleAttr ) );
            } while ( std::prev_permutation( bitmask.begin(), bitmask.end() ) );

//        } while( ... );// MAX ROWS PER RULE == 0
//        //REMOVE CORESPONDINNG RULES FROMM SUBSET AND DO IT AGAIN UNITL SUBSET IS NOT EMPTY
//    }
      return vector<CRule>();
}

CRule CILARulesExtractor::GetRuleTemplate( std::string bitmask )
{
    vector<pair<int,int>> attributes;
    for (unsigned i = 0; i < bitmask.size(); ++i) // [0..N-1] integers
    {
        if (bitmask[i])
        {
            std::cout << " " << i;
            attributes.push_back( pair<int,int>(i, 0) );
        }
    }
    CRule result;
    result.SetRule( attributes, -1 );
    return result;
}

bool CILARulesExtractor::GetNextRule( const CDataSet& data, CRule& rule )
{
    auto rules = rule.GetRules();
    for ( pair<int,int>& r : rules )
    {
        r.second++;
        if( r.second == data.GetAtributesClassCount( r.first ) )
        {
            r.second = 0;
        }
        else
        {
            return true;
        }
    }
    return false;
}
