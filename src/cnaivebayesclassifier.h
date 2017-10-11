#ifndef CNAIVEBAYESCLASSIFIER_H
#define CNAIVEBAYESCLASSIFIER_H

#include "cbaseclassifier.h"

class CNaiveBayesClassifier : public CBaseClassifier
{
public:
    CNaiveBayesClassifier();

    virtual void fit( const CDataSet& trainData ) override;
    virtual vector<int> predict( const CDataSet& testData ) override;
};

#endif // CNAIVEBAYESCLASSIFIER_H
