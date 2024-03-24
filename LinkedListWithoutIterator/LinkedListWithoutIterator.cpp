#include "Vec4.h"//sample class for tests
#include "LinkedListWithoutIterator.h"
#include <iostream>

int main()
{
	LinkedList<Vec4> myList;	

    for(int i = 0; i < 10; ++i)
        myList.pushFront({i,i,i,i});

    myList.eraseElement({9,9,9,9});

    myList.printList();

	return 0;
}