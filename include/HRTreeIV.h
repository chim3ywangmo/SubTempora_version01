#pragma once
#include<iostream>
#include<unordered_map>
#include<list>
#include<vector>
#include <map>
#include "HRTree.h"
#include "Timer.h"
#include "ewah/ewah.h"
using namespace std;

class HRTreeIV {
public:
    Timer t;
    double compressionTime = 0.0;
    vector<double> times;
    double compressedSizeIV=0.0;
    double originalSizeIV=0.0;
    typedef ewah::EWAHBoolArray<uint32_t> bitmap;
    int count = 0;
    int nodeSize =0;
    vector<uint32_t> graphInfoSize;
    vector<uint32_t> union_vec(vector<uint32_t> vec1, vector<uint32_t> vec2);
    map<int, vector<vector<uint32_t>>> convertgraphInfoNew(map<int, vector<vector<uint32_t>>> graphInfo);
    map<int, vector<vector<uint32_t>>> convertGraphInfo(std::map<int, std::vector<int>> gI);
    map<int, vector<bitmap>> convertGraphInfoBM(std::map<int, std::vector<vector<uint32_t>>> graphInfoIV);
    std::map<int, std::vector<vector<uint32_t>>> compressGraphInfoIV(std::map<int, std::vector<vector<uint32_t>>> graphInfo);
    vector<double> run(bool compress, HRTree* t, int option);
    void DFSConvert(HRNode* currentNode, int option);

};