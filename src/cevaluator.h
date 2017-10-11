#ifndef CEVALUATOR_H
#define CEVALUATOR_H

#include "cdataset.h"
#include "cbaseclassifier.h"

struct CStats
{
    double precision;
    double recall;
    double accuracy;
    double fscore;
};

class CEvaluator
{

public:
    CEvaluator() = delete;
    static CStats CrossValidation(CBaseClassifier& classifier, const CDataSet& dataSet , int fold = 10);

};

#endif // CEVALUATOR_H
