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

    void SetNormalDistribution( bool a_bDist );

private:
    virtual void clear() override;

    std::vector<std::pair<double, double> > NormalDistributionParameters(const CDataSet &data, const int attrID);
    std::vector<std::vector<double>> CountOccurences(const CDataSet &data, const int attrID );
    int CountUnique( const CDataSet& data, int attrID );
    double LogNormalDistribution(unsigned attrID, unsigned classID, double x);


    std::vector<double> _aClassProbability;
    std::vector<std::vector<std::vector<double>>> _aAttrProbability;
    std::vector<std::vector<std::pair<double,double>>> _aAttrNormalDistribution;
    bool m_bNormalDistribution = false;

};

#endif // CNAIVEBAYESCLASSIFIER_H
