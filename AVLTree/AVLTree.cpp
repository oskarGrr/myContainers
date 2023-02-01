#include "AVLTree.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <vector>

int main()
{  
    AVLTree<int> T1;

    T1.insert(8);
    T1.insert(5);
    T1.insert(9);
    T1.insert(3);
    T1.insert(6);
    T1.insert(11);
    T1.insert(2);
    T1.insert(4);
    T1.insert(7);
    T1.insert(10);
    T1.insert(12);
    T1.insert(1);

    T1.erase(10);

    auto T2(T1);//make a copy of T1 called T2

    //both trees should be
    //          5
    //        /   \
    //       3     8
    //      / \   /  \
    //     2   4 6    11
    //    /       \   / \   
    //   1         7 9   12

    //print both trees
    int c = 0;
    for(auto& i : T1)
        std::cout << i << " : " << ++c <<'\n';

    std::cout << "\n\n";

    int x = 0;
    for(auto& i : T2)
        std::cout << i << " : " << ++x <<'\n';
}