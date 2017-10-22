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
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 0.0\n";
            precision = 0.0;
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
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 0.0\n";
            recall = 0.0;
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
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 0.0\n";
            accuracy = 0.0;
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
            std::cerr << __PRETTY_FUNCTION__ << " Unable to calculate stats! Div zero! Taking 0.0\n";
            fscore = 0.0;
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
    enum EAverageStat
    {
        AVG_GLOBAL,
        AVG_UNWEIGHTED,
        AVG_WEIGHTED
    };

    CEvaluator() = delete;
    static CStats StratifiedCrossValidation(CBaseClassifier *classifier, const CDataSet &dataSet, unsigned fold = 10);
    static CStats CrossValidation(CBaseClassifier* classifier, const CDataSet& dataSet , unsigned fold = 10);
    static void SetAverageMode( const EAverageStat& a_eMode );

    static CStats CalculateStatsWeighted(const std::vector<int> &predictedValues, const CDataSet &targetDataSet);
    static CStats CalculateStatsUnweighted(const std::vector<int> &predictedValues, const CDataSet &targetDataSet);
    static CStats CalculateStatsGlobal(const std::vector<int> &predictedValues, const CDataSet &targetDataSet);
private:
    static CStats CalculateStats(const std::vector<int> &predictedValues, const CDataSet &targetDataSet);
    static std::vector<std::vector<int>> ConfusionMatrix(const std::vector<int> &predictedValues, const CDataSet &targetDataSet, int classID = -1 );
    static CStats CalculateAverageStat(const std::vector<CStats> &a_aStats);

    static EAverageStat _eMode;
};

#endif // CEVALUATOR_H
