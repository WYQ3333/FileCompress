#include <iostream>
#include"HuffmanTree.hpp"

void TestHuffmanTree(){
    HuffmanTree<int> ht;
    std::vector<int> array = {0, 1, 3, 5, 7};
    ht.CreateHuffmanTree(array, array[0]);
    auto temp = ht.GetRoot();
    std::cout << temp->_weight << std::endl;
}

int main()
{
    TestHuffmanTree();
    return 0;
}

