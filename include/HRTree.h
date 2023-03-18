#pragma once
#include<iostream>
#include<unordered_map>
#include<map>
#include<list>
#include<vector>
#include<string>
#include "ewah/ewah.h"

using namespace std;
class HRNode {
public:
    int graphid, vertexid;
    std::vector<int> label;
    std::unordered_map<int, HRNode*> children;
    std::map<int, std::vector<int>> graphInfo;
    std::map<int, std::vector<vector<uint32_t>>> graphInfoIV;
    std::map<int, std::vector<uint32_t>> HRTreeorigSize;
    typedef ewah::EWAHBoolArray<uint32_t> bitmap;
    std::map<int, vector<bitmap>> graphInfoBM;
    bool leaf;
    double compressedSize=0.0;
    int cCount=0;


    HRNode() {
        label.resize(1);
        label.push_back(-1);
        leaf = false;
    }
    HRNode(std::vector<int> s, bool isLeaf, int gid, int vid) {
        this->label.resize(s.size());
        this->graphid = gid;
        this->vertexid = vid;
        this->label = s;
        this->leaf = isLeaf;
        std::vector<int> arr(s.size());
        std::fill(arr.begin(), arr.end(), 0);
        graphInfo[gid] = arr;
    }

    HRNode(std::vector<int> s, bool isLeaf, int gid) {
        label.resize(s.size());
        label = s;
        leaf = isLeaf;
        std::vector<int> arr(s.size());
        std::fill(arr.begin(), arr.end(), 0);
        graphInfo[gid] = arr;
    }

};
class HRTree {
public:
    int gid;
    HRNode* root;
    bool start = true;
    HRTree() {
        root = new HRNode();
    }

    void setGraphInfo(map<int, vector<vector<uint32_t>>> graphInfoIV, HRNode* currentNode);
    void incrementCountOne(int vid, std::vector<int>s, HRNode* currentNode, HRNode* node, int initial, bool childExists = false);
    void incrementCount(int vid, std::vector<int>s, HRNode* currentNode, HRNode* node, int initial);
    void insertOne(std::vector<int> s, HRNode* node, int vid, int initial);
    void insertAll(std::vector<int> s, HRNode* node, int vid, int initial);
    bool search(std::vector<int> s, HRNode* node);
    std::vector<int> vectorSlice(int x, int y, std::vector<int> s1);
    std::vector<int> findLongestCommonPrefix(std::vector<int> s1, std::vector<int> s2);

};