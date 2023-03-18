#include "../include/HRTreeIV.h"
//#include "../include/FastPFor/codecfactory.h"
//#include "../include/FastPFor/deltautil.h"
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

vector<uint32_t> HRTreeIV::union_vec(vector<uint32_t> vec1, vector<uint32_t> vec2){
    vector<uint32_t> merged_vec;
    std::merge(vec1.begin(), vec1.end(),
                vec2.begin(), vec2.end(),
                std::back_inserter(merged_vec));

    vector<uint32_t>::iterator pte = std::unique(merged_vec.begin(), merged_vec.end());
    merged_vec.erase(pte, merged_vec.end());

    return merged_vec;
}

map<int, vector<vector<uint32_t>>> HRTreeIV::convertgraphInfoNew(map<int, vector<vector<uint32_t>>> graphInfo){
  map<int, vector<vector<uint32_t>>> graphInfoNew;
  vector<vector<uint32_t>> oldGI;
  int maxSize=0;
  for (auto iter = graphInfo.rbegin(); iter != graphInfo.rend(); ++iter) {
    if(iter->second.size()>maxSize){
        maxSize=iter->second.size();
        oldGI.resize(maxSize);
    }
    graphInfoNew[iter->first].resize(maxSize);
    for(int i=0; i<maxSize; i++){
      if(iter->second.size()<maxSize && i==maxSize-1){
          oldGI[i]=union_vec(oldGI[i], {});
      }else{
          oldGI[i]=union_vec(oldGI[i], iter->second[i]);
      }
      graphInfoNew[iter->first][i]=oldGI[i];
    }    
  }
  return graphInfoNew;
}

map<int, vector<vector<uint32_t>>> HRTreeIV::convertGraphInfo(std::map<int, std::vector<int>> gI) {
    map<int, vector<vector<uint32_t>>> graphInfo;
    for (auto k : gI) {
        for (uint32_t i = 0; i < k.second.size(); i++) {
            graphInfo[k.second[i]].resize(k.second.size());
            graphInfo[k.second[i]][i].push_back(k.first);
            int graphInfoSize = graphInfo[k.second[i]].size();
            if (graphInfo[k.second[i]][graphInfoSize-1].size()==0) {
                graphInfo[k.second[i]].pop_back();
            }
        }
    }

    nodeSize+=1; 
    for(auto n: graphInfo){
        for(int i=0; i<n.second.size(); i++){
            graphInfoSize.push_back(n.second[i].size());           
        }
    }

    graphInfo=convertgraphInfoNew(graphInfo);
    return graphInfo;
};


map<int, vector<HRTreeIV::bitmap>> HRTreeIV::convertGraphInfoBM(std::map<int, std::vector<vector<uint32_t>>> graphInfoIV){
    map<int, vector<bitmap>> graphInfoBM;
    for (auto k : graphInfoIV) {
        for (uint32_t i = 0; i < k.second.size(); i++) {
            sort(k.second[i].begin(), k.second[i].end());
            graphInfoBM[k.first].resize(k.second.size());
            if (k.second[i].size() != 0) {
                bitmap bitset1;
                for (auto i : k.second[i]) {
                    bitset1.set((size_t) i);
                }
                graphInfoBM[k.first][i]=bitset1;
            }
        }
    }

    return graphInfoBM;
}

std::map<int, std::vector<vector<uint32_t>>> HRTreeIV::compressGraphInfoIV(std::map<int, std::vector<vector<uint32_t>>> graphInfo) {
    std::map<int, std::vector<vector<uint32_t>>> compressedgraphInfoIV;
    using namespace SIMDCompressionLib;
    SIMDCompressionLib::IntegerCODEC& codec = *CODECFactory::getFromName("s4-fastpfor-d1");
    for (auto gI : graphInfo) { 
        for (uint32_t i = 0; i < gI.second.size(); i++) {
            sort(gI.second[i].begin(), gI.second[i].end());
            vector<uint32_t> original_data= gI.second[i]; 
            std::vector<uint32_t> compressed_output(gI.second[i].size() + 3072); 
            size_t compressedsize = compressed_output.size();
            codec.encodeArray(gI.second[i].data(), gI.second[i].size(), compressed_output.data(), compressedsize);
            compressed_output.resize(compressedsize);
            compressed_output.shrink_to_fit();
            compressedgraphInfoIV[gI.first].resize(gI.second.size());
            compressedgraphInfoIV[gI.first][i]=compressed_output;
        }
    }
    return compressedgraphInfoIV;
}

vector<double> HRTreeIV::run(bool compress, HRTree* t, int option) {
    vector<double> times;
    if (compress) {
        DFSConvert(t->root, option);
        times.push_back(compressionTime);
    }
    else {
        times = {0.00001};
    }
    return times;
};

void HRTreeIV::DFSConvert(HRNode* currentNode, int option) {
    if (currentNode->children.empty()) {
        return;
    }
    else {
        for (const auto& value : currentNode->children) {
            HRNode* node = value.second;
            if (option >= 4) {
                node->graphInfoIV  = convertGraphInfo(node->graphInfo);
                count++;
                t.start();
                node->graphInfoBM = convertGraphInfoBM(node->graphInfoIV);
                t.stop();
                DFSConvert(node, option);
                compressionTime = compressionTime + t.elapsedMilliseconds();
            }
            else if(option==1) {
                node->graphInfoIV = convertGraphInfo(node->graphInfo);            
                for(auto k: node->graphInfoIV){
                    node->HRTreeorigSize[k.first].resize(k.second.size());
                    for(int i=0; i<k.second.size(); i++){
                        node->HRTreeorigSize[k.first][i]=k.second[i].size();
                    }
                }
                t.start();
                node->graphInfoIV = compressGraphInfoIV(node->graphInfoIV);
                node->compressedSize = compressedSizeIV;
                t.stop();
                compressionTime = compressionTime + t.elapsedMilliseconds();
                DFSConvert(node, option);
            }
        }
    }
};
