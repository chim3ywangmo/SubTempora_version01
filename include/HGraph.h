#pragma once
#include<iostream>
#include "HRTree.h"
#include<unordered_map>
#include<map>
#include<vector>
#include<list>
#include<stack>

using namespace std;

class Node {
public:
    int vid, label;
    list<int> neighbors;

    Node(int vid, int label) {
        this->vid = vid;
        this->label = label;
    }
};

class HGraph {
public:
    int graphgid;
    unordered_map<int, Node*> m;
    HRTree t;
    vector<int> path;
    int initial = -1;

    HGraph() {
    }
    HGraph(int gid, map <int, int> vertexdict, vector<vector<int>> edges, HRTree t);
    void addedge(int x1, int x2, bool undir = true);
    void printadjlist();
    int deg(int vid);
    int getnumvertices();
    int getnumedges();
    bool child(int node, bool* visited);
    void DFSHelper(int node, stack<int> s, vector<int> path, bool* visited, int max_depth, int depth, bool qIndex);
    vector<int> DFSHelper(int node, bool* visited, int max_depth, int depth, bool qIndex);
    void DFS(int source, int max_depth, bool qIndex);
    void callDFS(int source, int max_depth, bool qIndex);
    vector<int> generatelabeledpath(vector<int> idpath);
    void insert(vector<int> idpath, int initial, bool qIndex);
};