#include <iostream>

#include "cdataset.h"
#include "cevaluator.h"
#include "cnaivebayesclassifier.h"
#include <ctime>

//using namespace std;

int main()
{
    std::srand(std::time(0));

    CDataSet data;
//    for ( auto i = 2; i < 5; i += 1 )
    {
        CNaiveBayesClassifier* pBayes = new CNaiveBayesClassifier();
//        data.Load( "data/iris.data", "%f %f %f %f %s", ',' );
//        data.Load( "data/seeds.data", "%f %f %f %f %f %f %f %s", ',' );
        data.Load( "data/ecoli.data", "%s %f %f %s %s %f %f %f %s", ',' );
//        data.Load( "data/ukm.data", "%f %f %f %f %f %s", ',' );
//        data.Load( "data/ukm.data", "%s %s %s %s %s %s", ',' );
//        data.Load( "data/heart.data", "%f %s %s %f %f %s %s %f %s %f %s %s %f %s", ',' );
//        data = data.Cut(0, 20);
//        for ( auto s : data.SplitByClasses() )
//        {
//            std::cout << s;
//        }
        data.Discretize( CDataSet::DISCRETIZATION_INTERVAL, 5 );
//        std::cout << data;
//        CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
//        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );
//        CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
//        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );
        CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );
        delete pBayes;

    }
    return 0;
}
