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
        data.Load( "data/iris.data", "%f %f %f %f %s", ',' );
//        data = data.Cut(0, 5);
        data.Discretize( CDataSet::DISCRETIZATION_INTERVAL, 3);
        std::cout << data;
        std::cout << CEvaluator::CrossValidation( pBayes, data );
        delete pBayes;

    }
    return 0;
}
