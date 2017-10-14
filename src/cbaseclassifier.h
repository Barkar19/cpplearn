#ifndef CBASECLASSIFIER_H
#define CBASECLASSIFIER_H

#include "cdataset.h"

class CBaseClassifier
{
public:
    CBaseClassifier();
    virtual ~CBaseClassifier();

    virtual void fit( const CDataSet& trainData ) = 0;
    virtual std::vector<int> predict( const CDataSet& testData ) = 0;
};

#endif // CBASECLASSIFIER_H
