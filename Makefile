test_Filecompress:test_Filecompress.cpp FileCompress.cpp FileCompress.h HuffmanTree.hpp
	g++ $^ -o $@ -std=c++11
.PHONY:
clean:
	rm test_Filecompress
