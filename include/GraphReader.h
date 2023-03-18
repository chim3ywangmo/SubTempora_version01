#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "HGraph.h"
#include "Timer.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include "ewah/ewah.h"
//#include "LabelMap.h"

using namespace std;
class GraphReader {
public:
    HRTree t;
    static int optionType;
    double time = 0;
    vector<int> edL;
    map<int, int> vertexDict;
    vector<vector<int>> edges; 
    GraphReader(){};
    std::map<std::string, int> _labelMap;
    GraphReader(const std::string& filename, HRTree t, int maxLength, int maxGID, bool qIndex);
    typedef ewah::EWAHBoolArray<uint32_t> bitmap;
    
    static void writeHere(std::ostream& os, HRNode& node) {
        size_t cCount=node.children.size();
        os.write((char*)&cCount,sizeof(size_t));
        bool strCheck=true;
        std::string s;
        int word;
        if(node.label.size()>1){
            stringstream ss;
            copy( node.label.begin(), node.label.end(), ostream_iterator<int>(ss, ""));
            s = ss.str();
            s = s.substr(0, s.length()-1);  
        }
        else{
            word=node.label[0];
            strCheck=false;
        }
        
        os.write((char*)&(strCheck), sizeof(bool));
        if(strCheck){
            size_t sizel=s.size();
            os.write((char*)&sizel, sizeof(size_t)); 
            os.write((char*)&s[0], sizel);
        }
        else{
            os.write((char*)&(word), sizeof(size_t));
        }

      if (optionType == 0) {
        int sizeG = node.graphInfo.size(); 
        os.write((char*)&sizeG, sizeof(int));

        if(strCheck){
            for(auto IT=node.graphInfo.begin(); IT!=node.graphInfo.end();IT++){
                os.write((char*)&(IT->first), sizeof(int)); 
                int sizeV=IT->second.size();
                for(int i=0; i<sizeV;i++){
                    os.write((char*)&(IT->second[i]), sizeof(int));
                }
            }
        }else{
            for(auto IT=node.graphInfo.begin(); IT!=node.graphInfo.end();IT++){
                os.write((char*)&(IT->first), sizeof(int)); 
                int sizeV=IT->second.size();
                for(int i=0; i<sizeV;i++){
                    os.write((char*)&(IT->second[i]), sizeof(int));
                }
            }
        }
        }
        else if (optionType == 1 || optionType==3) {
            int sizeG = node.graphInfoIV.size(); 
            os.write((char*)&sizeG, sizeof(int));

            if(strCheck){
                for(auto IT=node.graphInfoIV.begin(); IT!=node.graphInfoIV.end();IT++){
                    os.write((char*)&(IT->first), sizeof(int)); 
                    int sizeV=IT->second.size();
                    for(int vecI=0; vecI<sizeV; vecI++){
                        int sizeVec=IT->second[vecI].size();
                        os.write((char*)&(sizeVec), sizeof(int)); 
                        for(int i=0; i<sizeVec;i++){
                            os.write((char*)&(IT->second[vecI][i]), sizeof(int));
                        }
                    }
                }
            }else{
                for(auto IT=node.graphInfoIV.begin(); IT!=node.graphInfoIV.end();IT++){
                    os.write((char*)&(IT->first), sizeof(int));
                    int sizeV=IT->second.size();
                    for(int vecI=0; vecI<sizeV; vecI++){
                        int sizeVec=IT->second[vecI].size();
                        for(int i=0; i<sizeVec;i++){
                            os.write((char*)&(IT->second[vecI][i]), sizeof(int));
                        }
                    }
                }
            }

        }
        else if (optionType==4){
            int sizeVec=node.graphInfoBM.size();
            os.write((char*)&(sizeVec), sizeof(int));
            if(strCheck){
                for(auto n: node.graphInfoBM){
                    os.write((char*)&(n.first), sizeof(int)); 
                    sizeVec=n.second.size();
                    os.write((char*)&(sizeVec), sizeof(int)); 
                    for (auto iterI = n.second.begin(); iterI != n.second.end();) {
                        os.write((char*)&(*iterI), sizeof(int));
                        ++iterI;
                    }
                }
            }
            else{
                for(auto n: node.graphInfo){
                    os.write((char*)&(n.first), sizeof(int)); 
                    for (auto iterI = n.second.begin(); iterI != n.second.end();) {
                        os.write((char*)&(*iterI), sizeof(int));
                        ++iterI;
                    }
                }
            }

        }
    }


    static void ReadHere(std::istream& is, HRNode& node) {
        int csCount=0;
        is.read((char*)&(csCount), sizeof(size_t));
        node.cCount=csCount;
        bool sCheck=0;
        is.read((char*)&(sCheck), sizeof(bool)); 
        int sSize=0;
        int word=0;
        if(sCheck){
            is.read((char*)&(sSize), sizeof(int));
            for(int j=0; j<sSize; j++){
                int letter=0;
                is.read((char*)&(letter),sizeof(int));
                node.label.push_back(letter);
            }
        }
        else{
            is.read((char*)&(word), sizeof(int));
            node.label.push_back(word);
        }
        
        if (optionType == 0) {
        int sizeG=0; 
        is.read((char*)&sizeG, sizeof(int));
        if(sCheck){
            for(auto IT=0; IT!=sizeG;IT++){
                int gID=0;
                is.read((char*)&(gID), sizeof(int)); 
                int occID=0;
                vector<int> occIDs;
                for(int i=0; i<sSize;i++){
                    is.read((char*)&(occID), sizeof(int));
                    occIDs.push_back(occID);
                }
                node.graphInfo.insert(std::pair<int, vector<int>> (gID, occIDs));
            }
        }else{
            for(auto IT=0; IT!=sizeG;IT++){
                int gID=0;
                is.read((char*)&(gID), sizeof(int)); 
                int occID=0;
                vector<int> occIDs;
                is.read((char*)&(occID), sizeof(int));
                occIDs.push_back(occID);
                node.graphInfo.insert(std::pair<int, vector<int>> (gID, occIDs));
            }
        }
        }
        else if (optionType == 1 || optionType==3) {
            int sizeO = 0; 
            is.read((char*)&sizeO, sizeof(int));

            if(sCheck){
                for(auto IT=0; IT!=sizeO;IT++){
                    int occID=0;
                    is.read((char*)&(occID), sizeof(int)); 
                    int gIDSize=0;
                    int gID=0;
                    vector<uint32_t> gIDs;
                    vector<vector<uint32_t>> gIDList;
                    for(int i=0; i<sSize;i++){
                        is.read((char*)&(gIDSize), sizeof(int));
                        for(int k=0;k<gIDSize;k++){
                            is.read((char*)&(gID), sizeof(int));
                            gIDs.push_back(gID);
                        }
                        gIDList.push_back(gIDs);
                    }
                    node.graphInfoIV.insert(std::pair<int, vector<vector<uint32_t>>> (gID, gIDList));
                }
            }else{
                for(auto IT=0; IT!=sizeO;IT++){
                    int occID=0;
                    is.read((char*)&(occID), sizeof(int)); 
                    int gIDSize=0;
                    vector<vector<uint32_t>> gIDList;
                    int gID=0;
                    vector<uint32_t> gIDs;
                    for(int i=0; i<1;i++){
                        is.read((char*)&(gIDSize), sizeof(int));
                        for(int k=0;k<gIDSize;k++){
                            is.read((char*)&(gID), sizeof(int));
                            gIDs.push_back(gID);
                        }
                        gIDList.push_back(gIDs);
                    }
                    node.graphInfoIV.insert(std::pair<int, vector<vector<uint32_t>>>(gID, gIDList));
                }
            }

        }
        else if (optionType==4){
            int sizeVec=0;
            is.read((char*)&(sizeVec), sizeof(int));
            if(sCheck){
                for(int k=0; k<sSize; k++){
                    int sizeV=0, vVal=0;
                    vector<bitmap> vVec;
                    is.read((char*)&(vVal), sizeof(int)); 
                    is.read((char*)&(sizeV), sizeof(int)); 
                    bitmap bitset1;
                    for (auto iterI = 0; iterI != sizeV;) {
                        int value=0;
                        is.read((char*)&(value), sizeof(int));
                        bitset1.set((size_t) value);
                        ++iterI;
                    }
                    node.graphInfoBM[vVal][k]=bitset1;
                }
            }
            else{
                for(int k=0; k<1; k++){
                    int sizeV=0;
                    is.read((char*)&(sizeV), sizeof(int)); 
                    bitmap bitset1;
                    for (auto iterI = 0; iterI != 1;) {
                        int value=0;
                        is.read((char*)&(value), sizeof(int));
                        bitset1.set((size_t) value);
                        ++iterI;
                    }
                    node.graphInfoBM[sizeV][k]=bitset1;
                }
            }

        }
    }

    friend std::ostream& operator<< (std::ostream& os, HRNode& b) {
        os.write((char*)&(b), sizeof(uint32_t));
        return os;
    }

    friend std::ostream& operator<<(ostream& os, int c) {
        os << c;
        return os;
    }
    static void writeHRTree(std::ostream& os, HRNode& node);
    static void writeHRTree(std::ostream& os, HRTree& t, int option=0);
    static void readHRTree(std::istream& is, HRNode& node);
    static void readHRTree(std::istream& is, HRTree& t, int option=0);
    void createGraph(int gid, map<int, int> vDict, vector<vector<int>> ed, int maxLength, bool qIndex);

};