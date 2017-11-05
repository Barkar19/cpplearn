#include "cilarulesextractor.h"
#include <algorithm>
#include <iostream>
#include "crule.h"

CILARulesExtractor::CILARulesExtractor()
{

}

void CILARulesExtractor::Fit(const CDataSet &trainData)
{
    clear();
    _aRules = ExtractRules( trainData );
}

std::vector<int> CILARulesExtractor::Predict(const CDataSet &testData)
{
    std::vector<int> result( testData.GetSize(), -1 );
    for ( unsigned idx = 0; idx < testData.GetSize(); idx++ )
    {
        auto attributes = testData.AtributesAt( idx );
        std::vector<int> votes( testData.GetTargetMapSize(), 0 );
        for ( auto r : _aRules )
        {
            if ( ApplyRule( attributes, r) )
            {
                result[ idx ] = r.GetClass();
                votes[ r.GetClass() ]++;
            }
        }
//        if ( votes[0] > 0 || votes[1] > 0 || votes[2] > 0)
//            cout << votes[0] << " " << votes[1] << " " << votes[2] << "\n";
        if ( votes[0] || votes[1] || votes[2] )
        {
            result[ idx ] = std::max_element( votes.begin(), votes.end() ) - votes.begin();
        }
        else
        {
//            std::cerr << "NO RULE FOUND. ASUME MOST FREQUENT CLASS\n";
            result[ idx ] = _mostFrequentClass;
        }
    }
    return result;
}

void CILARulesExtractor::clear()
{
    _mostFrequentClass = -1;
    _aRules.clear();
}

bool CILARulesExtractor::ApplyRule( vector<int> values, CRule rule )
{
    bool result = true;
    auto rules = rule.GetRules();
    for ( const auto & r : rules )
    {
        const auto& attrID = r.first;
        const auto& attrValue = r.second;
        if ( values[attrID] != attrValue )
        {
            result = false;
            break;
        }
    }
    return result;
}

vector<CRule> CILARulesExtractor::ExtractRules(const CDataSet &data)
{
    vector<CRule> results;
    const vector<CDataSet> subsets = data.SplitByClasses();
    unsigned max = 0;
    for ( unsigned j = 0; j < subsets.size(); j++ )
    {
        if ( max < subsets[j].GetSize() )
        {
            max = subsets[j].GetSize();
            _mostFrequentClass = j;
        }
    }
    for( unsigned i = 0; i < subsets.size(); ++i ) // FOR EACH SUBSET
    {
        CDataSet currentSubset = subsets[i];
        CDataSet restOfData;
        for ( unsigned j = 0; j < subsets.size(); j++ )
        {
            if ( i != j )
            {
                restOfData.Merge( subsets[j] );
            }
        }
//        cout << "CURRENT DATA: " << currentSubset;
//        cout << "REST DATA: " << restOfData;
//        // SET OF ATTRIBUTES ( K OF N, for K = 1 ... N)
//        // MAX ROWS PER RULE = 0
        do
        {

            unsigned attributesCount = 0;
            CRule bestRule;
            unsigned maxRows = 0;
            do
            {
                attributesCount += 1;
                std::string bitmask( attributesCount, '1');
                bitmask.resize( data.GetAttributesSize(), '0');
                do
                {
//                    cout << "BITMASK:" << bitmask << endl << endl;
                    CRule currentRuleAttr = GetRuleTemplate( bitmask );
                    do
                    {
//                        cout << currentRuleAttr;
//                        cout << "COUNT RULE: " << data.CountIf( currentRuleAttr.GetRules() )<<endl;
                        if ( restOfData.CountIf( currentRuleAttr.GetRules() ) == 0 )
                        {
                            const unsigned rows = currentSubset.CountIf( currentRuleAttr.GetRules() );
                            if ( rows > maxRows )
                            {
                                maxRows = rows;
                                bestRule = currentRuleAttr;
                            }
                        }
                    } while ( GetNextRule(data, currentRuleAttr ) );
                } while ( std::prev_permutation( bitmask.begin(), bitmask.end() ) );
            } while( attributesCount != data.GetAttributesSize() && !maxRows );// MAX ROWS PER RULE == 0
//        //REMOVE CORESPONDINNG RULES FROMM SUBSET AND DO IT AGAIN UNITL SUBSET IS NOT EMPTY
        bestRule.SetClass( currentSubset.GetAtributes().back().back());
        currentSubset.Filter( bestRule.GetRules() );
        results.push_back( bestRule );
        }while( currentSubset.GetSize() != 0 );
    }
    return results;
}

CRule CILARulesExtractor::GetRuleTemplate( std::string bitmask )
{
    vector<pair<int,int>> attributes;
    for (unsigned i = 0; i < bitmask.size(); ++i) // [0..N-1] integers
    {
        if (bitmask[i] == '1')
        {
            attributes.push_back( pair<int,int>(i, 0) );
        }
    }
    CRule result;
    result.SetRule( attributes, -1 );
    return result;
}

bool CILARulesExtractor::GetNextRule( const CDataSet& data, CRule& rule )
{
    auto& rules = rule.GetRules();
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
