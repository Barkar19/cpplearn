#include "cnaivebayesclassifier.h"
#include <algorithm>
#include <iostream>

CNaiveBayesClassifier::CNaiveBayesClassifier()
{

}

CNaiveBayesClassifier::~CNaiveBayesClassifier()
{

}

void CNaiveBayesClassifier::fit(const CDataSet &trainData)
{
    _aClassProbability = std::vector<double>( trainData.GetTargetMap().size(), 0.0 );

    for ( auto c : trainData.GetTargetValues() )
    {
        _aClassProbability[ c ]++;
    }
    // [ ATTRIBUTE ] [ CLASS ] [ TARGET_CLASS ]
    _aAttrProbability = std::vector<std::vector<std::vector<double>>>( trainData.GetAttributesSize() );
//    _aAttrSets = std::vector< std::set<int> >( trainData.GetAttributesSize() );
//    FillAttributes( trainData );
    for ( unsigned i = 0; i < trainData.GetAttributesSize(); ++i )
    {
        _aAttrProbability[i] = CountOccurences( trainData, i );
        for ( auto& attributeClass : _aAttrProbability[i] )
        {
            for ( unsigned targetClass = 0; targetClass < attributeClass.size(); targetClass++ )
            {
                attributeClass[ targetClass ] /= _aClassProbability[ targetClass ];
            }
        }
    }
    for ( auto& value : _aClassProbability )
    {
        value /= double( trainData.GetSize() );
    }
}

//void CNaiveBayesClassifier::FillAttributes( const CDataSet &trainData )
//{
//    const auto& aAtributes = trainData.GetAtributes();
//    for ( unsigned i =0; i < _aAttrProbability.size(); ++i )
//    {
//        _aAttrSets[i] = std::set<int>( aAtributes[i].begin(), aAtributes[i].end() );
//        _aAttrProbability[i] = std::vector<std::vector<double>>( _aAttrSets[i].size() );
//        FillClasses( aAtributes[i], _aAttrProbability[i], _aAttrSets[i] );
//    }
//}

//void CNaiveBayesClassifier::FillClasses( const std::vector<int> &atributeValues, std::vector<std::vector<double>>& attributeProbability, std::set<int>& classes )
//{
//    std::vector<int> cl( classes.begin(), classes.end() );
//    for( unsigned i = 0; i < cl.size(); ++i )
//    {
//        attributeProbability[i] = std::vector<double>( _iTargetClassesCount );
//        FillProbability( atributeValues, attributeProbability, attributeProbability[i], cl[i]);
//    }
//}

//void CNaiveBayesClassifier::FillProbability( const std::vector<int> &atributeValues,
//                                             std::vector<std::vector<double>>& attribute,
//                                             std::vector<double>& targetClass,
//                                             int classID )
//{
//    for( unsigned i = 0; i < targetClass.size(); ++i )
//    {
//        targetClass[i] = std::count( atributeValues.begin(), atributeValues.end(), classID );
//    }
//}

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

std::vector<int> CNaiveBayesClassifier::predict(const CDataSet &testData)
{
    std::vector<int> v( testData.GetTargetValues().size(), 0 );
//    std::random_shuffle( v.begin(), v.end() );

    return v;
}
