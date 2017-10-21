#include "cnaivebayesclassifier.h"
#include <algorithm>
#include <iostream>
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
    _aClassProbability = std::vector<double>( trainData.GetTargetMap().size(), 0.0 );

    for ( auto c : trainData.GetTargetValues() )
    {
        _aClassProbability[ c ]++;
    }
    // [ ATTRIBUTE ] [ CLASS ] [ TARGET_CLASS ]
    _aAttrProbability = std::vector<std::vector<std::vector<double>>>( trainData.GetAttributesSize() );

    for ( unsigned i = 0; i < trainData.GetAttributesSize(); ++i )
    {
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
    int targetSize = data.GetTargetMap().size();

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

int CNaiveBayesClassifier::CountUnique( const CDataSet &data , int attrID )
{
    return data.GetAtributesClassCount( attrID );
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
                probabilities[classID] += _aAttrProbability[attrID][testData.ValueToClass(attrID, value)][classID];
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
}
