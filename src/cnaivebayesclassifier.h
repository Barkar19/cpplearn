#ifndef CNAIVEBAYESCLASSIFIER_H
#define CNAIVEBAYESCLASSIFIER_H

#include <set>

#include "cbaseclassifier.h"

class CNaiveBayesClassifier : public CBaseClassifier
{
public:
    CNaiveBayesClassifier();
    virtual ~CNaiveBayesClassifier();
    virtual void Fit( const CDataSet& trainData ) override;
    virtual std::vector<int> Predict( const CDataSet& testData ) override;

private:
    virtual void clear() override;

    std::vector<std::vector<double>> CountOccurences(const CDataSet &data, const int attrID );
    int CountUnique( const CDataSet& data, int attrID );


    std::vector<double> _aClassProbability;
    std::vector<std::vector<std::vector<double>>> _aAttrProbability;

};

#endif // CNAIVEBAYESCLASSIFIER_H
