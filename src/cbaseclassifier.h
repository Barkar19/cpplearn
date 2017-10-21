#ifndef CBASECLASSIFIER_H
#define CBASECLASSIFIER_H

#include "cdataset.h"

class CBaseClassifier
{
public:
    CBaseClassifier();
    virtual ~CBaseClassifier();

    virtual void Fit( const CDataSet& trainData ) = 0;
    virtual std::vector<int> Predict( const CDataSet& testData ) = 0;

protected:
    virtual void clear() = 0;
};

#endif // CBASECLASSIFIER_H
