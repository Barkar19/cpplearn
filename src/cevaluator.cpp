#include "cevaluator.h"
#include <algorithm>

CEvaluator::CEvaluator()
{

}

CStats CEvaluator::CrossValidation(CBaseClassifier &classifier, const CDataSet &dataSet, int fold)
{
    unsigned foldSize = dataSet.GetSize() / fold;
    for( unsigned i = 0; i < fold; ++i)
    {
        CDataSet trainData( dataSet );
        CDataSet testData = trainData.Cut( i * foldSize, ( i + 1 ) * foldSize );

        classifier.fit( trainData );
        vector<int> predictedClasses = classifier.predict( testData );
        CalculateStats( predictedClasses, testData.GetTargetValues() );
    }
}

CStats CEvaluator::CalculateStats( const vector<int>& predicted, const vector<int>& target)
{
    unsigned truePositives = count_if( predicted.begin(), predicted.end(), [&](int i){ return target[i] == i;} );
    unsigned trueNegatives = 0;
    unsigned falsePositives = 0;
    unsigned falseNegatives = 0;

}
