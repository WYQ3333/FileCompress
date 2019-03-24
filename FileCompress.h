#pragma once
#include<stdio.h>
#include<errno.h>
#include<iostream>
#include<string>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include"HuffmanTree.hpp"

using namespace std;

typedef struct Char_info{
    size_t _char_count;         //字符出现的次数
    char _ch;                   //字符
    std::string _char_code;          //字符编码
    bool operator>(const Char_info& temp)const{
        return _char_count > temp._char_count;
    }
    bool operator<(const Char_info& temp)const{
        return _char_count < temp._char_count;
    }
    bool operator==(const Char_info& temp)const{
        return _char_count == temp._char_count;
    }
    bool operator!=(const Char_info& temp)const{
        return _char_count != temp._char_count;
    }
    Char_info operator+(Char_info temp){
        Char_info cur;
        cur._char_count = _char_count + temp._char_count;
        return cur;
    }
}Char_info;




class FileCompress
{
public:
    FileCompress();
    void CompressFile(const std::string& strPath);
    void Printf_str_code();   //for test huffmancode
    void GetHuffmanCode(HuffmanTreeNode<Char_info>* pRoot);
    void WriteProfix(FILE* DestFile, const std::string& Filename);
    void UNCompressFile(const std::string& strPath);
    ~FileCompress();

private:
    std::vector<Char_info> char_info;
};

