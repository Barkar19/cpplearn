#include "cevaluator.h"
#include <algorithm>
#include <iostream>
#include <numeric>

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

std::vector<std::vector<int> > CEvaluator::ConfusionMatrix( const std::vector<int> &predictedValues, const CDataSet &targetDataSet, int classID )
{
    const auto& targetValues = targetDataSet.GetTargetValues();
    const auto& targetMap = targetDataSet.GetTargetMap();
    const int matrixSize = targetMap.size() - ( classID != -1 );
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

CStats CEvaluator::CalculateStats(const std::vector<int>& predictedValues, const CDataSet& targetDataSet )
{
    const auto& targetMap = targetDataSet.GetTargetMap();
    std::vector<CStats> aClassStats( targetMap.size(), CStats() );

    for( unsigned classID = 0; classID < targetMap.size(); classID++ )
    {
        const auto matrix = ConfusionMatrix(predictedValues, targetDataSet, classID );

        const int truePositives  = matrix[0][0];
        const int trueNegatives  = matrix[1][1];
        const int falsePositives = matrix[0][1];
        const int falseNegatives = matrix[1][0];
#ifdef DEBUG_EVALUATOR_CLASS
        std::cout << "CLASS: " << targetMap.at( classID ) << std::endl;
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
        std::cout << "CLASS: " << targetMap.at( classID ) << "\nSTAT: " << aClassStats[ classID ] << std::endl;
#endif
    }
    return CalculateAverageStat( aClassStats );
}


