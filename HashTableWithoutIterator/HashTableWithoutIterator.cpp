#include "HashTableWithoutIterator.h"
#include "Vec4.h"
#include <string>
#include <iostream>

#include <numeric>

int main()
{
	//seperate chaining hash table<key, value>
	SCHashTable<std::string, uint64_t> table;

    table.insert("cody",  127442000);
    table.insert("ryan",  212123235);
    table.insert("bob",   293819023);
    table.insert("riley", 134767566);
    table.insert("grant", 235253454);
    table.insert("name",  656856856);
    table.insert("sally", 111111111);
    table.insert("josh",  222222222);

    table.printTable();

	return 0;
}