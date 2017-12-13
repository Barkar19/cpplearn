#ifndef CKNNCLASSIFIER_H
#define CKNNCLASSIFIER_H

/*
 * LICZBA NAJBLIZSZYCH SĄSIADÓW ( K zawsze!!! nieparzyste)
 * MIARY ODLEGŁOŚCI
 *
 * NORMALIZACJA
 * GŁOSOWANIE NA RÓWNOLICZNE KLASY -> najczęstszą, losową
 * większościowe, ważone
 *
 *
*/

#include "cbaseclassifier.h"


struct Neighbour
{
    Neighbour():idx(-1),distance(-1.0),classID(-1){}
    Neighbour(int id,double dist,int c):idx(id),distance(dist),classID(c){}
    int idx;
    double distance;
    int classID;
};


class CKNNClassifier : public CBaseClassifier
{
public:

    enum EMetric
    {
        METRIC_MANHATTAN,
        METRIC_EUCLIDEAN,
        METRIC_CHEBYSHEV,
        METRIC_MINKOWSKI
    };

    enum EVotingAlgorithm
    {
        VOTE_SIMPLE,
        VOTE_WEIGHTED,
        VOTE_RANKED,
        VOTE_RANKEDWEIGHTED
    };

    CKNNClassifier();



    virtual void Fit( const CDataSet& trainData ) override;
    virtual std::vector<int> Predict( const CDataSet& testData ) override;

    void SetMetric( const EMetric & metric, double p = 0.0);
    void SetK(unsigned k);
    void SetVoting( const EVotingAlgorithm & voting );
private:

    int GetVotingResult( vector<Neighbour> neighbours );
    int VotingSimple(vector<Neighbour> neighbours);
    int VotingWeighted(vector<Neighbour> neighbours);
    int VotingRanked(vector<Neighbour> neighbours);
    int VotingRankedWeighted(vector<Neighbour> neighbours);
    double distance( const vector<double> a, const vector<double> b, double p);
    virtual void clear() override;

    CDataSet _aData;

    double _dMetric = 2.0;
    unsigned _iK = 3;
    EVotingAlgorithm _eVoting = VOTE_SIMPLE;

};

#endif // CKNNCLASSIFIER_H
