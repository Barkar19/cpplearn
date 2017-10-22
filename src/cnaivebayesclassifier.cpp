#include "cnaivebayesclassifier.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>

CNaiveBayesClassifier::CNaiveBayesClassifier()
{

}

CNaiveBayesClassifier::~CNaiveBayesClassifier()
{

}

void CNaiveBayesClassifier::Fit(const CDataSet &trainData)
{
    clear();
    _aClassProbability = std::vector<double>( trainData.GetTargetMapSize(), 0.0 );

    for ( auto c : trainData.GetTargetValues() )
    {
        _aClassProbability[ c ]++;
    }
    // [ ATTRIBUTE ] [ CLASS ] [ TARGET_CLASS ]
    _aAttrProbability = std::vector<std::vector<std::vector<double>>>( trainData.GetAttributesSize() );
    _aAttrNormalDistribution = std::vector<std::vector<std::pair<double,double>>>( trainData.GetAttributesSize() );

    for ( unsigned i = 0; i < trainData.GetAttributesSize(); ++i )
    {
        if ( trainData.GetAttributesType( i ) == CDataSet::ATTR_REAL )
        {
            _aAttrNormalDistribution[i] = NormalDistributionParameters(trainData, i);
        }
        _aAttrProbability[i] = CountOccurences( trainData, i );
        for ( auto& attributeClass : _aAttrProbability[i] )
        {
            for ( unsigned targetClass = 0; targetClass < attributeClass.size(); targetClass++ )
            {
                attributeClass[ targetClass ]  = std::log10( (attributeClass[ targetClass ] + 1.0) / ( _aClassProbability[ targetClass ] + _aAttrProbability[i].size() ) );
            }
        }
    }
    for ( auto& value : _aClassProbability )
    {
        value =  log10( value / double( trainData.GetSize() ) );
    }
}

std::vector <std::vector<double>> CNaiveBayesClassifier::CountOccurences(const CDataSet& data, const int attrID )
{
    int attrSize = data.GetAtributesClassCount( attrID );
    int targetSize = data.GetTargetMapSize();

    const auto& attributeValues = data.GetAtributes()[ attrID ];
    const auto& targetValues = data.GetTargetValues();

    std::vector<std::vector<double>> result( attrSize, std::vector<double>( targetSize, 0.0 ) );
    for ( auto targetIt = targetValues.begin(),
               attrIt = attributeValues.begin();
          attrIt != attributeValues.end();
          targetIt++, attrIt++
        )
    {
        const int attrClass = data.ValueToClass( attrID, *attrIt );
        result[ attrClass ][ *targetIt ]++;
    }
    return result;
}

std::vector<std::pair <double,double>> CNaiveBayesClassifier::NormalDistributionParameters(const CDataSet& data, const int attrID )
{
    unsigned targetSize = data.GetTargetMapSize();

    const auto& attributeValues = data.GetRealAtribute( attrID );
    const auto& targetValues = data.GetTargetValues();

    std::vector< std::pair<double,double> >  result( targetSize, std::pair<double,double>() );
    std::vector< std::vector<double>> values( targetSize );
    auto targetIt = targetValues.begin();
    auto attrIt = attributeValues.begin();
    for ( ;
          attrIt != attributeValues.end();
          targetIt++, attrIt++
        )
    {
        values[ *targetIt ].push_back( *attrIt );
    }

    for ( unsigned i =0; i < targetSize; ++i )
    {
        result[i].first = std::accumulate( values[i].begin(), values[i].end(), 0.0,
                                           [=](double sum, double x){ return sum + x / values[i].size();});
        result[i].second = std::sqrt( std::accumulate( values[i].begin(), values[i].end(), 0.0,
                                            [=](double sum, double x){ return sum + (x - result[i].first)*(x - result[i].first) / values[i].size();}) );
//        std::cout <<"ATTR ID " << attrID << " CLASS ID " << i << " MEAN: " << result[i].first  << " STD: " << result[i].second <<std::endl;
    }
    return result;
}

int CNaiveBayesClassifier::CountUnique( const CDataSet &data , int attrID )
{
    return data.GetAtributesClassCount( attrID );
}

double CNaiveBayesClassifier::LogNormalDistribution( unsigned attrID, unsigned classID, double x )
{
    const auto mean = _aAttrNormalDistribution[ attrID ][ classID ].first;
    const auto sd = _aAttrNormalDistribution[ attrID ][ classID ].second;

    const auto exponent = - ( pow(x - mean, 2.0) / ( 2.0 * pow( sd, 2.0) ) );
    const auto p = ( 1.0 / ( ( sqrt(2.0 * M_PI) * sd ) ) ) * exp( exponent );
    return log10(p);
}

std::vector<int> CNaiveBayesClassifier::Predict(const CDataSet &testData)
{
    std::vector<int> result( testData.GetSize() );
    for ( unsigned idx = 0; idx < testData.GetSize(); idx++ )
    {
        std::vector<double> probabilities( _aClassProbability.size(), 1.0 );
        for( unsigned classID = 0; classID < _aClassProbability.size(); ++classID )
        {
            const auto attrValues = testData.AtributesAt( idx);
            for( unsigned attrID = 0; attrID < _aAttrProbability.size(); ++attrID )
            {
                const double value = attrValues[ attrID ];
                if ( false )
                {
                    probabilities[classID] += _aAttrProbability[attrID][testData.ValueToClass(attrID, value)][classID];
                }
                else
                {
                    probabilities[classID] += LogNormalDistribution( attrID, classID, value );
                }
            }
            probabilities[classID] += _aClassProbability[classID];
        }
        auto it = std::max_element(probabilities.begin(), probabilities.end());
        result[ idx ] = it - probabilities.begin();
    }
    return result;
}

void CNaiveBayesClassifier::clear()
{
    _aClassProbability.clear();
    _aAttrProbability.clear();
    _aAttrNormalDistribution.clear();
}
