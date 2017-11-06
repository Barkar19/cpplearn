#include <iostream>

#include "cdataset.h"
#include "cevaluator.h"
#include "cnaivebayesclassifier.h"
#include "cilarulesextractor.h"
#include <ctime>
#include <sstream>
#include <chrono>

using namespace std;

int main()
{
    std::cout.setf(std::ios::unitbuf);
    std::srand(std::time(0));
    std::vector<std::pair<string,string>> dataString;
//    dataString.push_back( pair<string,string>( "data/ila_xor.data", "%s %s %s %s" ) );
//    dataString.push_back( pair<string,string>( "data/seasons.data", "%s %s %s %s" ) );
//    dataString.push_back( pair<string,string>( "data/iris.data", "%f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/seeds.data", "%f %f %f %f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/ecoli.data", "%f %f %f %f %f %f %f %s" ) );
    dataString.push_back( pair<string,string>( "data/ukm.data", "%f %f %f %f %f %s" ) );
//    dataString.push_back( pair<string,string>( "data/heart.data", "%f %s %s %f %f %s %s %f %s %f %s %s %f %s" ) );

    for ( auto data : dataString )
    {
        CDataSet* pDataSet = new CDataSet;
        pDataSet->Load( data.first, data.second, ',' );
        for ( unsigned i = 2; i <= 20; i+=2 )
        {
            CBaseClassifier* pClassifier = new CILARulesExtractor();

            vector<CDataSet::EDiscretizationType> dis = { CDataSet::DISCRETIZATION_INTERVAL,
                                                          CDataSet::DISCRETIZATION_FREQUENCY };

            for ( auto t : dis )
            {
                pDataSet->Discretize( t, i);
                std::stringstream ss;
                string out;
                std::cerr << data.first << ",discrete"<<t<<",cross,w," << i <<"\n";

                auto start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
                    ss << data.first << ",discrete"<<t<<",cross,w," << i <<","\
                              << CEvaluator::CrossValidation( pClassifier, *pDataSet );
                    ss >> out;
                    std::cout << out << std::endl;
                auto end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
                    ss << data.first << ",discrete"<<t<<",cross,u," << i <<","\
                              << CEvaluator::CrossValidation( pClassifier, *pDataSet);
                    ss >> out;
                    std::cout << out << std::endl;
                end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
                    ss  << data.first << ",discrete"<<t<<",cross,g," << i <<","\
                              << CEvaluator::CrossValidation( pClassifier, *pDataSet );
                    ss >> out;
                    std::cout << out << std::endl;
                end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_WEIGHTED );
                    ss << data.first << ",discrete"<<t<<",stratified,w," << i <<","\
                              << CEvaluator::StratifiedCrossValidation( pClassifier, *pDataSet );
                    ss >> out;
                    std::cout << out << std::endl;
                end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_UNWEIGHTED );
                    ss << data.first << ",discrete"<<t<<",stratified,u," << i <<","\
                              << CEvaluator::StratifiedCrossValidation( pClassifier, *pDataSet );
                    ss >> out;
                    std::cout << out << std::endl;
                end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

                start = std::chrono::system_clock::now();
                    CEvaluator::SetAverageMode( CEvaluator::AVG_GLOBAL );
                    ss << data.first << ",discrete"<<t<<",stratified,g," << i <<","\
                              << CEvaluator::StratifiedCrossValidation( pClassifier, *pDataSet );
                    ss >> out;
                    std::cout << out << std::endl;
                end = std::chrono::system_clock::now();
                std::cerr<< "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

            }

            delete pClassifier;
        }

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
