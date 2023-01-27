#include "AVLTree.h"
#include <vector>
#include <ctime>
#include <iostream>

int main()
{  
    std::srand(std::time(nullptr));
    AVLTree<int> avlTree;

    for(int i = 0; i < 11; ++i)
        avlTree.insert(i);

    avlTree.erase(5);
    avlTree.erase(4);
    avlTree.erase(6);

    int c = 0;
    for(auto i : avlTree) 
        std::cout << i << " : " << c++ <<'\n';

    avlTree.clearTree();
}