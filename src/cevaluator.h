#ifndef CEVALUATOR_H
#define CEVALUATOR_H

#include "cdataset.h"
#include "cbaseclassifier.h"

#include <iostream>

struct CStats
{
    CStats():precision(0),recall(0),accuracy(0),fscore(0){}
    void SetPrecision( int tp, int /*tn*/, int fp, int /*fn*/)
    {
        if ( tp + fp )
        {
            precision   = double( tp ) / double ( tp + fp );
        }
        else
        {
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 100%\n";
            precision = 1.0;
//            throw std::runtime_error( __PRETTY_FUNCTION__ + string(" Unable to calculate stats! Div zero!") );
        }
    }
    void SetRecall( int tp, int /*tn*/, int /*fp*/, int fn)
    {
        if ( tp + fn )
        {
            recall      = double( tp ) / double ( tp + fn );
        }
        else
        {
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 100%\n";
            recall = 1.0;
//            throw std::runtime_error( __PRETTY_FUNCTION__ + string(" Unable to calculate stats! Div zero!") );
        }
    }
    void SetAccuracy( int tp, int tn, int fp, int fn)
    {
        if ( tp + tn + fp + fn )
        {
            accuracy    = double( tp + tn ) / double ( tp + tn + fp + fn );
        }
        else
        {
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 100%\n";
            accuracy = 1.0;
//            throw std::runtime_error( __PRETTY_FUNCTION__ + string(" Unable to calculate stats! Div zero!") );
        }
    }
    void SetFScore()
    {
        if ( precision + recall != 0 )
        {
            fscore = ( 2 * precision * recall ) / ( precision + recall );
        }
        else
        {
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 100%\n";
            fscore = -1.0;
//            throw std::runtime_error( __PRETTY_FUNCTION__ + string(" Unable to calculate stats! Div zero!") );
        }
    }

    double precision;
    double recall;
    double accuracy;
    double fscore;

    friend std::ostream& operator<<( std::ostream& out, const CStats& s)
    {
        out << "PRECISION: "    << s.precision << std::endl;
        out << "RECALL: "       << s.recall << std::endl;
        out << "ACCURACY: "     << s.accuracy << std::endl;
        out << "FSCORE: "       << s.fscore << std::endl << std::endl;
        return out;
    }
};

class CEvaluator
{

public:
    CEvaluator() = delete;
    static CStats CrossValidation(CBaseClassifier* classifier, const CDataSet& dataSet , unsigned fold = 10);

private:
    static CStats CalculateStats(const std::vector<int> &predictedValues, const CDataSet &targetDataSet);
    static std::vector<std::vector<int>> ConfusionMatrix(const std::vector<int> &predictedValues, const CDataSet &targetDataSet, int classID = -1 );
    static CStats CalculateAverageStat(const std::vector<CStats> &a_aStats);
};

#endif // CEVALUATOR_H
