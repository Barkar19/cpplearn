#ifndef CNAIVEBAYESCLASSIFIER_H
#define CNAIVEBAYESCLASSIFIER_H

#include <set>

#include "cbaseclassifier.h"

class CNaiveBayesClassifier : public CBaseClassifier
{
public:
    CNaiveBayesClassifier();
    virtual ~CNaiveBayesClassifier();
    virtual void fit( const CDataSet& trainData ) override;
    virtual std::vector<int> predict( const CDataSet& testData ) override;

private:
    void FillAttributes(const CDataSet &trainData);
    void FillClasses(const std::vector<int> &atributeValues, std::vector<std::vector<double> > &attributeProbability, std::set<int> &classes);
    void FillProbability(const std::vector<int> &atributeValues, std::vector<std::vector<double> > &attribute, std::vector<double> &targetClass, int classID);
    std::vector<std::vector<double>> CountOccurences(const CDataSet &data, const int attrID );
    int CountUnique( const CDataSet& data, int attrID );


    std::vector<double> _aClassProbability;
    std::vector<std::vector<std::vector<double>>> _aAttrProbability;

};

#endif // CNAIVEBAYESCLASSIFIER_H
