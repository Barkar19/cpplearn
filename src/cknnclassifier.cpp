#include "cknnclassifier.h"

#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>

using namespace std;

CKNNClassifier::CKNNClassifier()
{

}

void CKNNClassifier::Fit(const CDataSet &trainData )
{
//    vector<double> test1 = {1.0, 2.0, 0.0};
//    vector<double> test2 = {1.0, 2.0, 0.0};

//    cout << "DISTANCE 1: " << distance(test1, test2, 1) << endl;
//    cout << "DISTANCE 2: " << distance(test1, test2, 2)<< endl;
//    cout << "DISTANCE 3: " << distance(test1, test2, 3)<< endl;
//    cout << "DISTANCE inf: " << distance(test1, test2, std::numeric_limits<double>::infinity())<< endl;

//    test1 = {0.0, 0.0, 0.0};
//    test2 = {1.0, 2.0, 0.0};

//    cout << "DISTANCE 1: " << distance(test1, test2, 1)<< endl;
//    cout << "DISTANCE 2: " << distance(test1, test2, 2)<< endl;
//    cout << "DISTANCE 3: " << distance(test1, test2, 3)<< endl;
//    cout << "DISTANCE inf: " << distance(test1, test2, std::numeric_limits<double>::infinity())<< endl;

//    test2 = {0.0, 0.0, 0.0};
//    test1 = {1.0, 2.0, 0.0};

//    cout << "DISTANCE 1: " << distance(test1, test2, 1)<< endl;
//    cout << "DISTANCE 2: " << distance(test1, test2, 2)<< endl;
//    cout << "DISTANCE 3: " << distance(test1, test2, 3)<< endl;
//    cout << "DISTANCE inf: " << distance(test1, test2, std::numeric_limits<double>::infinity())<< endl;

//    test1 = {0.0, 0.0};
//    test2 = {1.0, 1.0};

//    cout << "DISTANCE 1: " << distance(test1, test2, 1)<< endl;
//    cout << "DISTANCE 2: " << distance(test1, test2, 2)<< endl;
//    cout << "DISTANCE 3: " << distance(test1, test2, 3)<< endl;
//    cout << "DISTANCE inf: " << distance(test1, test2, std::numeric_limits<double>::infinity())<< endl;

    _aData = trainData;
}

std::vector<int> CKNNClassifier::Predict(const CDataSet &testData)
{
    /*
     *
     * Data normalization before Prediction and Fitting...
     *
     */

    std::vector<int> result( testData.GetSize() );
    for ( unsigned idx = 0; idx < testData.GetSize(); idx++ )
    {
        std::vector<double> values = testData.GetRealAtributeValues( idx );

        vector<Neighbour> neighbours( _aData.GetSize() );
        for ( unsigned n = 0; n < _aData.GetSize(); n++ )
        {
            neighbours[n] = Neighbour(n,
                                      distance(values, _aData.GetRealAtributeValues(n), _dMetric),
                                      _aData.GetTargetValues()[n]);
        }

        std::sort(neighbours.begin(), neighbours.end(),
            [](const Neighbour & a, const Neighbour & b) -> bool
        {
            return a.distance < b.distance;
        });

        neighbours.resize( _iK*4 );
        result[idx] = GetVotingResult( neighbours )
    }
    return result;
}

int CKNNClassifier::VotingSimple( vector<Neighbour> neighbours )
{
    std::map<int, int> votes;
    for ( auto n : neighbours )
    {
        votes[ n.classID ] += 1;
    }
    auto e = max_element( votes.begin(), votes.end(),
                 [](const pair<int,int> & a, const pair<int,int> & b) -> bool
                {
                    return a.second < b.second;
                });
    return (*e).first;
}

int CKNNClassifier::VotingWeighted( vector<Neighbour> neighbours )
{
    double minDistance = neighbours.front().distance;
    double maxDistance = neighbours.back().distance;

    for( auto& n : neighbours )
    {
        if( (maxDistance - minDistance ) != 0.0000001L )
        {
            n.distance = 2 - (n.distance - minDistance) / ( maxDistance - minDistance );
        }
        else
        {
            n.distance = 1.0;
        }
//        cout << n.classID << " = " << n.distance <<endl;
    }

    std::map<int, double> votes;
    for ( auto n : neighbours )
    {
        votes[ n.classID ] += n.distance;
    }
    auto e = max_element( votes.begin(), votes.end(),
                 [](const pair<int,double> & a, const pair<int,double> & b) -> bool
                {
                    return a.second < b.second;
                });

//    for ( pair<int,double> v : votes )
//    {
//        cout << v.first << "=>" << v.second <<endl;
//    }
//    cout << "============\n";
    return (*e).first;
}


int CKNNClassifier::VotingRanked( vector<Neighbour> neighbours )
{
    double minDistance = neighbours.front().distance;
    double maxDistance = neighbours.back().distance;

    for( unsigned i = 0; i < neighbours.size(); ++i )
    {
        Neighbour & n = neighbours[i];
        n.distance = (neighbours.size() - i);
//        cout << n.classID << " = " << n.distance <<endl;
    }

    std::map<int, double> votes;
    for ( auto n : neighbours )
    {
        votes[ n.classID ] += n.distance;
    }
    auto e = max_element( votes.begin(), votes.end(),
                 [](const pair<int,double> & a, const pair<int,double> & b) -> bool
                {
                    return a.second < b.second;
                });

//    for ( pair<int,double> v : votes )
//    {
//        cout << v.first << "=>" << v.second <<endl;
//    }
//    cout << "============\n";
    return (*e).first;
}

int CKNNClassifier::VotingRankedWeighted( vector<Neighbour> neighbours )
{
    double minDistance = neighbours.front().distance;
    double maxDistance = neighbours.back().distance;

    for( unsigned i = 0; i < neighbours.size(); ++i )
    {
        Neighbour & n = neighbours[i];
        if( (maxDistance - minDistance ) != 0.0000001L )
        {
            n.distance = (2 - (n.distance - minDistance) / ( maxDistance - minDistance ))*(neighbours.size() - i);
        }
        else
        {
            n.distance = (neighbours.size() - i);
        }
    }

    std::map<int, double> votes;
    for ( auto n : neighbours )
    {
        votes[ n.classID ] += n.distance;
    }
    auto e = max_element( votes.begin(), votes.end(),
                 [](const pair<int,double> & a, const pair<int,double> & b) -> bool
                {
                    return a.second < b.second;
                });

//    for ( pair<int,double> v : votes )
//    {
//        cout << v.first << "=>" << v.second <<endl;
//    }
//    cout << "============\n";
    return (*e).first;
}


void CKNNClassifier::SetMetric(const CKNNClassifier::EMetric &metric, double p)
{
    switch ( metric )
    {
    case METRIC_MANHATTAN:
        _dMetric = 1.0;
        break;
    case METRIC_EUCLIDEAN:
        _dMetric = 2.0;
        break;
    case METRIC_CHEBYSHEV:
        _dMetric = std::numeric_limits<double>::infinity();
        break;
    case METRIC_MINKOWSKI:
        _dMetric = p;
        break;
    }
}

void CKNNClassifier::SetK(unsigned k)
{
    if ( k <= _aData.GetSize() )
    {
        _iK = k;
    }
    else
    {
        std::cerr << "Too large K!";
        _iK = _aData.GetSize();
    }
}

void CKNNClassifier::SetVoting(const CKNNClassifier::EVotingAlgorithm &voting)
{
    _eVoting = voting;
}

int CKNNClassifier::GetVotingResult(vector<Neighbour> neighbours)
{
    int result = -1;
    switch( _eVoting )
    {
    case VOTE_SIMPLE:
        result = VotingSimple( neighbours );
        break;
    case VOTE_WEIGHTED:
        result = VotingWeighted( neighbours );
        break;
    case VOTE_RANKED:
        result = VotingRanked( neighbours );
        break;
    case VOTE_RANKEDWEIGHTED:
        result = VotingRankedWeighted( neighbours );
        break;
    }
    return result;
}

double CKNNClassifier::distance(const vector<double> a, const vector<double> b, double p)
{
    double result = std::nan("1");
    if ( std::isinf( p ) )
    {
        auto a_iter = a.begin();
        auto b_iter = b.begin();

        double acc = -1.0;

        for ( ;a_iter != a.end(); ++a_iter, ++b_iter )
        {
            if ( fabs(*a_iter-*b_iter) > acc )
            {
                acc = fabs(*a_iter-*b_iter);
            }
        }
        result = acc;
    }
    else if ( b.size() == a.size() )
    {
        auto a_iter = a.begin();
        auto b_iter = b.begin();

        double acc = 0.0;

        for ( ;a_iter != a.end(); ++a_iter, ++b_iter )
        {
            acc += pow( fabs(*a_iter - *b_iter), p);
        }
        result = pow ( acc, 1.0 / p );
    }
    return result;
}

void CKNNClassifier::clear()
{

}
