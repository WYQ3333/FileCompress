#include"FileCompress.h"


FileCompress::FileCompress() {
    //初始化vector<Char_info> char_info;
    char_info.resize(256);
    for(size_t i = 0; i < 256; ++i){
        char_info[i]._ch = i;
        char_info[i]._char_count = 0;
        char_info[i]._char_code = "";
    }
}
FileCompress::~FileCompress(){
}

void FileCompress::CompressFile(const std::string& strPath){
    //字符信息存放在结构体中
    if(strPath.empty()){
        perror("filePath is empty!! ");
        return;
    }
    //打开文件统计文件中字符出现的次数
    FILE* fsource = fopen(strPath.c_str(), "rb");
    if(nullptr == fsource){
        perror("open strPath failure!!");
        return;
    }
    char buff[1024];
    while(true){
        size_t ReadSize = 0;
        ReadSize = fread(buff, 1, 1024, fsource);
        if(ReadSize == 0){
            break;
        }
        for(size_t i = 0; i < ReadSize; ++i){
            char_info[buff[i]]._char_count++;
        }
    }
    //已经获得文档中字符出现的次数
    //根据字符出现的次数重建Huffman树
    HuffmanTree<Char_info> ht;
    ht.CreateHuffmanTree(char_info, char_info[0]);
    
    //获取huffman编码
    GetHuffmanCode(ht.GetRoot());
    //写压缩文件,设计压缩文件的文件名，与源文件相同，但后缀不同
    std::string profix = ".hzp";
    std::string temp = strPath;
    std::string strFinsh = temp.substr(0, temp.find('.', 0));
    std::string strCompress = strFinsh + profix;
    FILE* fwrDest = fopen(strCompress.c_str(), "wb");
    if(fwrDest == nullptr){
        perror("fopen CompressFile failure!!");
        return;
    }
    //写压缩文件的相关信息
    WriteProfix(fwrDest, temp);
    //通过编码重写文件
    //将fsource重置到文件的开头
    //要按位写入编码
    fseek(fsource, 0, SEEK_SET);
    char ch = 0;
    int bit_count = 8;
    while(true){
        size_t ReadSize = 0;
        ReadSize = fread(buff, 1, 1024, fsource);
        if(ReadSize == 0){
            break;
        }
        for(size_t i = 0; i < ReadSize; ++i){
            std::string temp_bit = char_info[buff[i]]._char_code;
            for(size_t j = 0; j < temp_bit.size(); ++j){
                if(temp_bit[j] == '1'){
                    ch |= 1;
                }
                ch <<= 1;
                bit_count++;            //记录已经写了多少位了
                if(bit_count == 8){
                    //满8位了,需要将这一个字节写入到文件中
                    fputc(ch, fwrDest);
                    bit_count = 0;
                }
            }
        }
    }
    if(bit_count > 0 && bit_count < 8){
        ch <<= (8 - bit_count);
        fputc(ch, fwrDest);
    }
    //关闭文件描述符
    fclose(fwrDest);
    fclose(fsource);
}

void FileCompress::WriteProfix(FILE* DestFile, const std::string& Filename){
    std::string str_suffix = Filename.substr(Filename.find('.'));       //取后缀
    str_suffix += "\n"; 
    std::string temp = "";
    int linecount = 0;
    /* for(size_t i =1; i < char_info.size(); ++i){ */
    /*     if(char_info[i]._char_count){ */
    /*         std::cout << char_info[i]._ch; */
    /*         std::cout << "--"; */
    /*         std::cout << char_info[i]._char_count << std::endl; */
    /*     } */
    /* } */
    for(size_t i = 1; i < char_info.size(); ++i){
        std::string str_ch = "";
        std::string str_count = "";
        if(char_info[i]._char_count){
            str_ch += char_info[i]._ch;
            str_count += std::to_string(char_info[i]._char_count);

            /* if(linecount == 1){ */
            /*     std::cout << str_ch.c_str() -  0 << " " << str_ch.c_str() -'0' << std::endl; */
            /* } */
            
            /* if(linecount == 2){ */
            /*     std::cout << str_ch.c_str() -  0 << " " << str_ch.c_str() -'0' << std::endl; */
            /* } */
            /* if(linecount == 3){ */
            /*     std::cout << str_ch.c_str() -  0 << " " << str_ch.c_str() -'0' << std::endl; */
            /* } */
            /* if(linecount == 4){ */
            /*     std::cout << str_ch.c_str() -  0 << " " << str_ch.c_str() -'0' << std::endl; */
            /* } */
            /* if(linecount == 5){ */
            /*     std::cout << str_ch.c_str() -  0 << " " << str_ch.c_str() -'0' << std::endl; */
            /* } */

            temp += str_ch + "/" + str_count + "\n";
            linecount++;
        }
    }
    std::cout << linecount << std::endl;
    str_suffix += std::to_string(linecount) + temp;
    fwrite(str_suffix.c_str(), 1, str_suffix.size(), DestFile);
}

void FileCompress::Printf_str_code(){
    for(size_t i = 0; i < char_info.size(); ++i){
        if(char_info[0] != char_info[i]){
            std::cout << char_info[i]._ch ;
            std::cout << "-->" ;
            std::cout << char_info[i]._char_count << std::endl;
        }
    }
}

void FileCompress::GetHuffmanCode(HuffmanTreeNode<Char_info>* pRoot){
    if(pRoot == nullptr){
        return;
    }
    GetHuffmanCode(pRoot->_Left);
    GetHuffmanCode(pRoot->_Right);
    if(pRoot->_Left == nullptr && pRoot->_Right == nullptr){
        HuffmanTreeNode<Char_info>* pCur = pRoot;
        HuffmanTreeNode<Char_info>* pParent = pCur->_Parent;
        std::string& str_code = char_info[pCur->_weight._ch]._char_code;
        while(pParent != nullptr){
            if(pCur == pParent->_Left){
                str_code += "0";
            }
            else{
                str_code += "1";
            }
            pCur = pParent;
            pParent = pCur->_Parent;
        }
        //此时获得的编码需要逆置之后才能使用
        reverse(str_code.begin(), str_code.end());
    }
    //Printf_str_code();
}



void FileCompress::UNCompressFile(const std::string& strPath){
    if(strPath.empty()){
        perror("filePath is empty!! ");
        return;
    }
}
