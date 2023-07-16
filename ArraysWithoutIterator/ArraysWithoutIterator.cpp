#include "Vec4.h"//sample class for tests
#include "DynamicArrayWithoutIterator.h"
#include "StaticArrayWithoutIterator.h"

int main()
{
	DynamicArray<Vec4> A1;
	//StaticArray<Vec4, 5> staticArray;

    //move 8 vec4's from the stack into the DynamicArray
    for(int i = 0; i < 8; ++i)
        A1.pushBack({i,i,i,i});

    for(int i = 0; i < A1.getSize(); ++i)
        std::cout << A1[i] << '\n';

    return 0;
}