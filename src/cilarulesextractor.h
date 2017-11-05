#ifndef CILARULESEXTRACTOR_H
#define CILARULESEXTRACTOR_H

#include <vector>
#include "crule.h"
#include "cdataset.h"

#include "cbaseclassifier.h"

using namespace std;

class CILARulesExtractor : public CBaseClassifier
{
public:
    CILARulesExtractor();


    virtual void Fit( const CDataSet& trainData ) override;
    virtual std::vector<int> Predict( const CDataSet& testData ) override;
    vector<CRule> ExtractRules( const CDataSet& data );
private:
    virtual void clear() override;

    CRule GetRuleTemplate(std::string bitmask);
    bool GetNextRule(const CDataSet &data, CRule& rule);
    bool ApplyRule( vector<int> values, CRule rule );

    vector<CRule> _aRules;
    int _mostFrequentClass = -1;
};

#endif // CILARULESEXTRACTOR_H
