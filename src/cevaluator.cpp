#include "cevaluator.h"
#include <algorithm>
#include <iostream>
#include <numeric>

CEvaluator::EAverageStat CEvaluator::_eMode = CEvaluator::AVG_UNWEIGHTED;

CStats CEvaluator::CrossValidation(CBaseClassifier* classifier, const CDataSet &dataSet, unsigned fold)
{
    unsigned foldSize = dataSet.GetSize() / fold;
    std::vector<CStats> aStats( fold );
    for( unsigned i = 0; i < fold; ++i)
    {
        CDataSet trainData( dataSet );
        CDataSet testData = trainData.Cut( i * foldSize, ( i + 1 ) * foldSize );

        classifier->Fit( trainData );
        std::vector<int> predictedClasses = classifier->Predict( testData );
        ConfusionMatrix( predictedClasses, testData );
        aStats[i] = CalculateStats( predictedClasses, testData );
//        std::cout << aStats[i];
    }
    return CalculateAverageStat( aStats );
}

void CEvaluator::SetAverageMode(const CEvaluator::EAverageStat &a_eMode)
{
    _eMode = a_eMode;
}

CStats CEvaluator::StratifiedCrossValidation(CBaseClassifier* classifier, const CDataSet &dataSet, unsigned fold)
{
    auto setsByClass = dataSet.SplitByClasses();
    std::vector<CStats> aStats( fold );

    for( unsigned i = 0; i < fold; ++i)
    {
        CDataSet trainData;
        CDataSet testData;
        for( auto set : setsByClass )
        {
            unsigned foldSize = set.GetSize() / fold;
            if( !foldSize ) foldSize = 1;
            CDataSet currentTrainData( set );
            unsigned low = (i * foldSize) % set.GetSize();
            unsigned up = low + foldSize;
            CDataSet currentTestData = currentTrainData.Cut( low, up );

            trainData.Merge( currentTrainData );
            testData.Merge( currentTestData );
        }
//        std::cout << testData;
        classifier->Fit( trainData );
        std::vector<int> predictedClasses = classifier->Predict( testData );
        ConfusionMatrix( predictedClasses, testData );
        aStats[i] = CalculateStats( predictedClasses, testData );
//        std::cout << aStats[i];
    }
    return CalculateAverageStat( aStats );
}

std::vector<std::vector<int> > CEvaluator::ConfusionMatrix( const std::vector<int> &predictedValues, const CDataSet &targetDataSet, int classID )
{
    const auto& targetValues = targetDataSet.GetTargetValues();
    const int matrixSize = ( classID == -1 ) ? targetDataSet.GetTargetMapSize() : 2;
    std::vector<std::vector<int>> matrix( matrixSize, std::vector<int>( matrixSize ) );

    for ( auto targetIt = targetValues.begin(),
               predictedIt = predictedValues.begin();
          predictedIt != predictedValues.end();
          targetIt++, predictedIt++
        )
    {
        if ( classID == -1 )
        {
            matrix[ *predictedIt ][ *targetIt ]++;
        }
        else
        {
            matrix[ *predictedIt != classID ]
                  [ *targetIt    != classID ]++;
        }
    }
#ifdef DEBUG_EVALUATOR
    std::cout << "CONF MATRIX\n";
    if ( classID == -1)
    for ( auto row : matrix )
    {
        for ( auto value : row )
        {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
#endif
    return matrix;
}

CStats CEvaluator::CalculateAverageStat( const std::vector<CStats>& a_aStats )
{
    CStats result;
    for ( const auto& s : a_aStats )
    {
        result.precision += s.precision;
        result.recall    += s.recall;
        result.accuracy  += s.accuracy;
        result.fscore    += s.fscore;
    }
    result.precision /= a_aStats.size();
    result.recall    /= a_aStats.size();
    result.accuracy  /= a_aStats.size();
    result.fscore    /= a_aStats.size();
    return result;
}

CStats CEvaluator::CalculateStatsWeighted( const std::vector<int>& predictedValues, const CDataSet& targetDataSet )
{
    std::vector<CStats> aClassStats( targetDataSet.GetSize(), CStats() );
    auto sets = targetDataSet.SplitByClasses();

    std::vector<unsigned> aWeights;
    for ( auto s : sets )
    {
        aWeights.push_back( s.GetSize() );
    }
    unsigned idx = 0;
    for( unsigned classID = 0; classID < targetDataSet.GetTargetMapSize(); classID++ )
    {
        const auto matrix = ConfusionMatrix(predictedValues, targetDataSet, classID );

        const int truePositives  = matrix[0][0];
        const int trueNegatives  = matrix[1][1];
        const int falsePositives = matrix[0][1];
        const int falseNegatives = matrix[1][0];
#ifdef DEBUG_EVALUATOR_CLASS
//        std::cout << "CLASS: " << targetMap.at( classID ) << std::endl;
#endif
#ifdef DEBUG_EVALUATOR
        std::cout << "TP: " << truePositives << std::endl;
        std::cout << "TN: " << trueNegatives << std::endl;

        std::cout << "FP: " << falsePositives << std::endl;
        std::cout << "FN: " << falseNegatives<< std::endl;
#endif
        for ( unsigned i = 0; i < aWeights[classID]; ++i )
        {
            aClassStats[ idx ].SetPrecision( truePositives, trueNegatives, falsePositives, falseNegatives );
            aClassStats[ idx ].SetRecall(    truePositives, trueNegatives, falsePositives, falseNegatives );
            aClassStats[ idx ].SetAccuracy(  truePositives, trueNegatives, falsePositives, falseNegatives );
            aClassStats[ idx ].SetFScore();
            idx++;
        }
#ifdef DEBUG_EVALUATOR
//        std::cout << "CLASS: " << targetMap.at( classID ) << "\nSTAT: " << aClassStats[ classID ] << std::endl;
#endif
    }
    return CalculateAverageStat( aClassStats );
}
CStats CEvaluator::CalculateStatsUnweighted( const std::vector<int>& predictedValues, const CDataSet& targetDataSet )
{
    std::vector<CStats> aClassStats( targetDataSet.GetTargetMapSize(), CStats() );

    for( unsigned classID = 0; classID < targetDataSet.GetTargetMapSize(); classID++ )
    {
        const auto matrix = ConfusionMatrix(predictedValues, targetDataSet, classID );

        const int truePositives  = matrix[0][0];
        const int trueNegatives  = matrix[1][1];
        const int falsePositives = matrix[0][1];
        const int falseNegatives = matrix[1][0];
#ifdef DEBUG_EVALUATOR_CLASS
//        std::cout << "CLASS: " << targetMap.at( classID ) << std::endl;
#endif
#ifdef DEBUG_EVALUATOR
        std::cout << "TP: " << truePositives << std::endl;
        std::cout << "TN: " << trueNegatives << std::endl;

        std::cout << "FP: " << falsePositives << std::endl;
        std::cout << "FN: " << falseNegatives<< std::endl;
#endif
        aClassStats[ classID ].SetPrecision( truePositives, trueNegatives, falsePositives, falseNegatives );
        aClassStats[ classID ].SetRecall(    truePositives, trueNegatives, falsePositives, falseNegatives );
        aClassStats[ classID ].SetAccuracy(  truePositives, trueNegatives, falsePositives, falseNegatives );
        aClassStats[ classID ].SetFScore();
#ifdef DEBUG_EVALUATOR
//        std::cout << "CLASS: " << targetMap.at( classID ) << "\nSTAT: " << aClassStats[ classID ] << std::endl;
#endif
    }
    return CalculateAverageStat( aClassStats );
}
CStats CEvaluator::CalculateStatsGlobal( const std::vector<int>& predictedValues, const CDataSet& targetDataSet )
{
    CStats result;
    unsigned truePositives  = 0;
    unsigned trueNegatives  = 0;
    unsigned falsePositives = 0;
    unsigned falseNegatives = 0;
    const auto matrix = ConfusionMatrix(predictedValues, targetDataSet );
#ifdef DEBUG_EVALUATOR
    std::cout << "CONF MATRIX\n";
    for ( auto row : matrix )
    {
        for ( auto value : row )
        {
            std::cout << value << "\t";
        }
        std::cout << std::endl;
    }
#endif
    for( unsigned i = 0; i < matrix.size(); ++i )
    {
        for( unsigned j = 0; j < matrix.back().size(); ++j )
        {
            if ( i == j )
            {
                truePositives += matrix[i][j];
            }
            else if ( i < j )
            {
                falsePositives += matrix[i][j];
            }
            else if ( i > j )
            {
                falseNegatives += matrix[i][j];
            }
        }
    }
    result.SetPrecision( truePositives, trueNegatives, falsePositives, falseNegatives );
    result.SetRecall(    truePositives, trueNegatives, falsePositives, falseNegatives );
    result.SetAccuracy(  truePositives, trueNegatives, falsePositives, falseNegatives );
    result.SetFScore();
    return result;
}


CStats CEvaluator::CalculateStats(const std::vector<int>& predictedValues, const CDataSet& targetDataSet )
{
    CStats result;
    switch( _eMode )
    {
        case AVG_WEIGHTED:
        {
            result = CalculateStatsWeighted( predictedValues, targetDataSet );
            break;
        }
        case AVG_UNWEIGHTED:
        {
            result = CalculateStatsUnweighted( predictedValues, targetDataSet );
            break;
        }
        case AVG_GLOBAL:
        {
            result = CalculateStatsGlobal( predictedValues, targetDataSet );
            break;
        }
    }
    return result;
}


