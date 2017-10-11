#include <iostream>

#include "cdataset.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    CDataSet data;

    data.Load( "data/iris.data", "%f %f %f %f %s", ',' );

    CDataSet copy( data);

    data = copy.Split(data.GetSize() - 3, data.GetSize() );

    data.Merge( copy.Split(3, 6) );
    data.Merge( data );
    data.Merge( copy.Split(0, 1) );

    cout << data;

    CDataSet other = data.Cut(0, 5);

    cout << data;
    cout << other;
    return 0;
}
