#ifndef CILARULESEXTRACTOR_H
#define CILARULESEXTRACTOR_H

#include <vector>
#include "crule.h"
#include "cdataset.h"

using namespace std;

class CILARulesExtractor
{
public:
    CILARulesExtractor();

    vector<CRule> ExtractRules( const CDataSet& data );
private:
    CRule GetRuleTemplate(std::string bitmask);
    bool GetNextRule(const CDataSet &data, CRule& rule);
};

#endif // CILARULESEXTRACTOR_H
