#include "Vec4.h"//sample class for tests
#include "DynamicArrayWithoutIterator.h"
#include "StaticArrayWithoutIterator.h"

int main()
{
	DynamicArray<Vec4> dynamicArray;
	//StaticArray<Vec4, 5> staticArray;

    dynamicArray.pushBack({1,1,1,1});
    dynamicArray.pushBack({2,2,2,2});
    dynamicArray.pushBack({3,3,3,3});
        
    dynamicArray.popBack();

    dynamicArray.pushBack({4,4,4,4});



    return 0;
}