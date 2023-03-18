#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "GraphReader.h"
#include <cstring>
#include "HRTreeIV.h"
#include <utility> 
#include <stdio.h>
#include "ewah/ewah.h"
#include "../include/SIMDPFor/binarypacking.h"
#include "../include/SIMDPFor/bitpackinghelpers.h"
#include "../include/SIMDPFor/codecfactory.h"
#include "../include/SIMDPFor/common.h"
#include "../include/SIMDPFor/compositecodec.h"
#include "../include/SIMDPFor/delta.h"
#include "../include/SIMDPFor/simdbinarypacking.h"
#include "../include/SIMDPFor/simdbitpackinghelpers.h"
#include "../include/SIMDPFor/simdfastpfor.h"
#include "../include/SIMDPFor/synthetic.h"
#include "../include/SIMDPFor/util.h"
#include "../include/SIMDPFor/variablebyte.h"
#include "../include/SIMDPFor/intersection.h"

using namespace std;
class QueueNode {
public:
    HRNode* fNode;
    HRNode* sNode;
    QueueNode* next;
    vector<int> qlabel;
    bool visited;
    int pos;

    QueueNode() {};
    QueueNode(HRNode* _qNode, HRNode* _gNode, vector<int> _qlabel, bool _visited, int position) {
        this->fNode = _qNode;
        this->sNode = _gNode;
        this->next = NULL;
        this->visited = _visited;
        this->pos = position;
        if (!_qlabel.empty()) {
            copy(_qlabel.begin(), _qlabel.end(), back_inserter(qlabel));

        }
    }
};

class BuildManager {
    public:
        HRTree qIndex;
        HRTree gIndex;
        HRTreeIV gIndexIV;
        ofstream myfile;
        std::set<uint32_t> candidateGraphSet;
        std::vector<uint32_t> candidateGraphSetIV;
        bool second = true;
        double time = 0;
        Timer timer;
        vector<double> type;
        std::vector<double> datasetType;
        vector<double> constructionTime;
        vector<double> compressionTime;
        vector<double> constructionSize;
        vector<double> maxSize;
        vector<double> maxLen;
        double sortingTime=0.0;
        std::map<std::string, int> _labelMap;

        BuildManager(){};
        BuildManager(const BuildManager& a){
            qIndex=a.qIndex;
            gIndex=a.gIndex;
            gIndexIV=a.gIndexIV;
            candidateGraphSet=a.candidateGraphSet;
            candidateGraphSetIV=a.candidateGraphSetIV;
            second=a.second;
            time=a.time;
            type=a.type;
            datasetType=a.datasetType;
            constructionTime=a.constructionTime;
            compressionTime=a.compressionTime;
            constructionSize=a.constructionSize;
            maxSize=a.maxSize;
            maxLen=a.maxLen;
            sortingTime=a.sortingTime;
        } 
        void Empty() {
            this->~BuildManager();
            new (this) BuildManager();
        }
        void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<double>>> dataset);
        void BuildGraphIndex(std::string file, std::string queryName, int maxLength, int maxGID, bool compress, int option=0, int datasetTyp=0);
        void BuildQueryIndex(std::string file, std::string queryName, int maxLength, int maxGID, bool compress, int option=0, int datasetTyp=0);
        std::vector<int> findLongestCommonPrefix(std::vector<int> s1, std::vector<int> s2);
        std::vector<int> vectorSlice(int x, int y, std::vector<int> s1);
        void match(int option);
        bool prune_rule_1(HRNode* qNode, HRNode* gNode, int position, int option);
        bool prune_rule_1(HRNode* qNode, HRNode* gNode, int position, std::set<uint32_t>::iterator cgs_IT);
        void prune_rule_2(HRNode* qNode, HRNode* gNode, int position);
        void prune_rule_3(HRNode* qNode, HRNode* gNode, int position);
        bool matched_nodes(HRNode* qNode, HRNode* gNode, int position, int option);
        vector<uint32_t> union_vec(vector<uint32_t> vec1, vector<uint32_t> vec2);
};