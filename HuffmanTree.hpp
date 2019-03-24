#pragma once
//模版不能进行分离编译
//解决方法是将声明和定义都放在hpp文件中

#include<iostream>
#include<string>
#include<vector>
#include<queue>
template<class T>
//模板结构体不能使用typedef ,原因是template中的类型不知道，但是typedef必须要知道类型才能重命名，所以报错
struct HuffmanTreeNode{
    //采用孩子双亲表示法，因为获取Huffman编码的时候需要其父亲结点
    typedef  HuffmanTreeNode HT;
    typedef HT* pHT;
    HuffmanTreeNode(const T& weight)
        :_Left(nullptr)
        ,_Right(nullptr)
        ,_Parent(nullptr)
        ,_weight(weight){}
    //采用孩子双亲表示法，因为获取Huffman编码的时候需要其父亲结点
    pHT _Left;
    pHT _Right;
    pHT _Parent;
    T _weight;
};

template<class T>
struct com{
    typedef  HuffmanTreeNode<T> HTNode;
    typedef HTNode* pHTNode;
    bool operator()(const pHTNode& left, const pHTNode& right){
        return left->_weight > right->_weight;
    }
};

template<class T>
class HuffmanTree
{
public:
    typedef  HuffmanTreeNode<T> HTNode;
    typedef HTNode* pHTNode;
    HuffmanTree()
        :_pRoot(nullptr)
    {}

    void CreateHuffmanTree(const std::vector<T>& v, const T& invalid){
        if(v.empty()){
            perror("The Char_info is False!!");
            return;
        }
        //根据v中的元素信息建立小堆，使用优先级队列
        //优先级队列默认比较方法是小于，所以生成的是大堆，因此需要自定义priority_queue的
        //比较方式
        std::priority_queue<pHTNode, std::vector<pHTNode>, com<T>> q;
        for(size_t i = 0; i < v.size(); ++i){
            if(v[i] != invalid){
                q.push(new HTNode(v[i]));
            }
        }
        //取堆顶元素，创建Huffman树
        while(q.size() != 1){
            pHTNode pleft = q.top();
            q.pop();
            pHTNode pright = q.top();
            q.pop();
            pHTNode pparent = new HTNode(pleft->_weight + pright->_weight);
            pparent->_Left = pleft;
            pparent->_Right = pright;
            pleft->_Parent = pparent;
            pright->_Parent = pparent;
            q.push(pparent);
        }
        _pRoot = q.top();
    }

    HTNode* GetRoot(){
        return _pRoot;
    }

    void _destoryHT(HuffmanTreeNode<T>* pCur){
        if(pCur == nullptr){
            return;
        }
        _destoryHT(pCur->_Left);
        _destoryHT(pCur->_Right);
        delete pCur;
        pCur = nullptr;
    }

    ~HuffmanTree() {
       _destoryHT(_pRoot); 
    }

private:
     pHTNode _pRoot;          //根节点
};

