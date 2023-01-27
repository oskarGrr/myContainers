#include "AVLTree.h"
#include <vector>
#include <ctime>
#include <iostream>

int main()
{  
    std::srand(std::time(nullptr));
    AVLTree<int> avlTree;

    for(int i = 0; i < 30; ++i)
    {
        avlTree.insert(i);
    }

    avlTree.erase(5);
    avlTree.erase(11);
    avlTree.erase(15);

    int c = 0;
    for(auto i : avlTree) 
        std::cout << i << " : " << c++ <<'\n';
}