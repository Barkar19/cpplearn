#include <iostream>

#include "cdataset.h"
#include "cevaluator.h"
#include "cnaivebayesclassifier.h"
#include <ctime>

using namespace std;

int main()
{
    std::srand(std::time(0));
    std::vector<std::pair<string,string>> dataString;
//    dataString.push_back( pair<string,string>( "data/iris.data", "%f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/seeds.data", "%f %f %f %f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/ecoli.data", "%s %f %f %f %f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/ukm.data", "%f %f %f %f %f %s" ) );
//    dataString.push_back( pair<string,string>( "data/heart.data", "%f %s %s %f %f %s %s %f %s %f %s %s %f %s" ) );

    for ( auto data : dataString )
    {
        CDataSet* pDataSet = new CDataSet;
        pDataSet->Load( data.first, data.second, ',' );
        for ( unsigned i = 2; i <= 20; ++i )
        {
            CNaiveBayesClassifier* pBayes = new CNaiveBayesClassifier();
            pBayes->SetNormalDistribution( false );

            vector<CDataSet::EDiscretizationType> dis = { CDataSet::DISCRETIZATION_INTERVAL,
                                                          CDataSet::DISCRETIZATION_FREQUENCY };

            for ( auto t : dis )
            {
                pDataSet->Discretize( t, i);

                std::cerr << data.first << ",discrete"<<t<<",cross,w," << i <<"\n";

                CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
                std::cout << data.first << ",discrete"<<t<<",cross,w," << i <<",";
                std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet );
                CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
                std::cout << data.first << ",discrete"<<t<<",cross,u," << i <<",";
                std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet);
                CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
                std::cout << data.first << ",discrete"<<t<<",cross,g," << i <<",";
                std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet );

                CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
                std::cout << data.first << ",discrete"<<t<<",stratified,w," << i <<",";
                std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );
                CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
                std::cout << data.first << ",discrete"<<t<<",stratified,u," << i <<",";
                std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );
                CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
                std::cout << data.first << ",discrete"<<t<<",stratified,g," << i <<",";
                std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );

            }

            delete pBayes;
        }
        CNaiveBayesClassifier* pBayes = new CNaiveBayesClassifier();
        pBayes->SetNormalDistribution( true );

        CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
        std::cout << data.first << ",norm,cross,w,-1,";
        std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet );
        CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
        std::cout << data.first << ",norm,cross,u,-1,";
        std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet);
        CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
        std::cout << data.first << ",norm,cross,g,-1,";
        std::cout << CEvaluator::CrossValidation( pBayes, *pDataSet );

        CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
        std::cout << data.first << ",norm,stratified,w,-1,";
        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );
        CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
        std::cout << data.first << ",norm,stratified,u,-1,";
        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );
        CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
        std::cout << data.first << ",norm,stratified,g,-1,";
        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, *pDataSet );

        delete pBayes;

        delete pDataSet;
    }

//    CDataSet data;
////    for ( auto i = 2; i < 5; i += 1 )
//    {
////        data.Load( "data/iris.data", "%f %f %f %f %s", ',' );
////        data.Load( "data/seeds.data", "%f %f %f %f %f %f %f %s", ',' );
////        data.Load( "data/ecoli.data", "%s %f %f %s %s %f %f %f %s", ',' );
////        data.Load( "data/ukm.data", "%f %f %f %f %f %s", ',' );
////        data.Load( "data/ukm.data", "%s %s %s %s %s %s", ',' );
//        data.Load( "data/heart.data", "%f %s %s %f %f %s %s %f %s %f %s %s %f %s", ',' );
////        data = data.Cut(0, 20);
////        for ( auto s : data.SplitByClasses() )
////        {
////            std::cout << s;
////        }
//        data.Discretize( CDataSet::DISCRETIZATION_INTERVAL, 5 );
////        std::cout << data;
//        CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
//        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );
//        CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
//        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );
//        CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
//        std::cout << CEvaluator::StratifiedCrossValidation( pBayes, data );

//    }
    return 0;
}
