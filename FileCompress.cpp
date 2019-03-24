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

void FileCompress::CompressFile(const std::string& strPath){
    //字符信息存放在结构体中
    if(strPath.empty()){
        perror("filePath is empty!! ");
        return;
    }
    //打开文件统计文件中字符出现的次数
    FILE* fd = fopen(strPath.c_str(), "r");
    if(nullptr == fd){
        perror("open strPath failure!!");
        return;
    }
    char buff[1024];
    while(true){
        size_t ReadSize = 0;
        ReadSize = fread(buff, 1024, 1, fd);
        if(ReadSize == 0){
            break;
        }
        for(size_t i = 0; i < ReadSize; ++i){
            char_info[buff[i]]._char_count++;
        }
    }
    //已经获得文档中字符出现的次数
    //根据字符出现的次数重建Huffman树
    
    //关闭文件描述符
    fclose(fd);
    
}


void FileCompress::UNCompressFile(const std::string& strPath){
    if(strPath.empty()){
        perror("filePath is empty!! ");
        return;
    }

}
