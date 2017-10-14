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

    data.Load( "data/iris.data", "%f %f %f %f %s", ',' );

    CDataSet copy( data);

    data = copy.Split(data.GetSize() - 12, data.GetSize() );

//    data.Merge( copy.Split(3, 6) );
//    data.Merge( data );
//    data.Merge( copy.Split(0, 1) );

    std::cout << data;

//    CDataSet other = data.Cut(0, 5);

//    cout << data;
//    cout << other;
    CNaiveBayesClassifier* pBayes = new CNaiveBayesClassifier();
    std::cout << CEvaluator::CrossValidation( pBayes, data, 4 );
    delete pBayes;
    return 0;
}
