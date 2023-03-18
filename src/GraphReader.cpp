#include "../include/GraphReader.h"
int GraphReader::optionType;

    GraphReader::GraphReader(const std::string& filename, HRTree t, int maxLength, int maxGID, bool qIndex) {
        this->t = t;
        std::ifstream file(filename);
        int gid = -1;
        char input;
        Timer timer;
        if (!file.is_open()) {
            std::cout << "Could not open" << endl;
        }
        else {
            while (file >> input) {
                if (input == 't') {
                    char input2, input3;
                    file >> input2 >> input3;
                    if (gid > -1) {
                        if (gid == maxGID) {
                            return;
                        }
                        timer.start();
                        createGraph(gid, vertexDict, edges, maxLength, qIndex);
                        timer.stop();
                        time = time + timer.elapsedMilliseconds();
                    }
                    gid += 1;
                    vertexDict.clear();
                    edges.clear();
                }
                if (input == 'v')
                {
                    int v, l, label_id;
                    file >> v >> l;
                    vertexDict[v] = l;
                    std::map<std::string, int>::iterator iter = _labelMap.find(to_string(l));
                    if (iter == _labelMap.end()) {
                        label_id = _labelMap.size();
                        _labelMap[to_string(l)] = label_id;
                    }
                    else {
                        label_id = iter->second;
                    }
                }
                if (input == 'e') {
                    int v1, v2;
                    file >> v1 >> v2;
                    edL.clear();
                    edL.push_back(v1);
                    edL.push_back(v2);
                    edges.push_back(edL); 
                }
            }

        }
    }

    void GraphReader::writeHRTree(std::ostream& os, HRNode& node) {            
        writeHere(os, node); 
        for (auto i : node.children) {
            HRNode& n = *(i.second);
            writeHRTree(os, n);
        }
    }

    void GraphReader::writeHRTree(std::ostream& os, HRTree& t, int option) {
        optionType= option;
        if (t.root != NULL) {
            writeHRTree(os, *(t.root));
        }
    }

    void GraphReader::readHRTree(std::istream& is, HRNode& node) {            
        ReadHere(is, node); 
        for (auto i=0; i<node.cCount;i++) {
            HRNode* nodePtr=new HRNode;
            HRNode& curNode= *nodePtr;
            readHRTree(is, curNode);
            node.children[curNode.label[0]]=&curNode;
        }
    }

    void GraphReader::readHRTree(std::istream& is, HRTree& t, int option) {
        optionType= option;
        t.root=new HRNode();
        readHRTree(is, *(t.root));
    }

    void GraphReader::createGraph(int gid, map<int, int> vDict, vector<vector<int>> ed, int maxLength, bool qIndex) {
        HGraph g(gid, vDict, ed, t); ;
        for (map<int, int>::iterator it = vertexDict.begin(); it != vertexDict.end(); ++it) {
             if(gid==1 && it->first == 0){
                 g.callDFS(it->first, maxLength, qIndex);     
             }
             else {
                 g.callDFS(it->first, maxLength, qIndex);
             }
        }
    }

