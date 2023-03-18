#include "../include/HGraph.h"

HGraph::HGraph(int gid, map <int, int> vertexdict, vector<vector<int>> edges, HRTree t) {
    this->graphgid = gid;
    t.gid = gid;
    for (auto vertexid : vertexdict) {
        m[vertexid.first] = new Node(vertexid.first, vertexid.second);
    }
    for (int k = 0; k < edges.size(); k++) {
        addedge(edges[k][0], edges[k][1]);
    }

    this->t = t;

}

void HGraph::addedge(int x1, int x2, bool undir) {
    m[x1]->neighbors.push_back(x2);
    if (undir) {
        m[x2]->neighbors.push_back(x1);
    }
}

void HGraph::printadjlist() {
    for (auto pair : m) {
        auto v = pair.first;
        Node* node = pair.second;
    }
}

int HGraph::deg(int vid) {
    return m[vid]->neighbors.size();
}

int HGraph::getnumvertices() {
    return m.size();
}

int HGraph::getnumedges() {
    int totaldegree = 0;
    for (int i = 0; i < getnumvertices(); i++) {
        totaldegree = totaldegree + deg(i);
    }
    return totaldegree / 2;
}

bool HGraph::child(int node, bool* visited) { 
    bool vst = false;
    for (auto n : m[node]->neighbors) {
        if (!visited[n]) {
            vst = true;
            return vst;
        }
        else {
            vst = false;
        }
    }
    return vst;
}

void HGraph::DFSHelper(int node, stack<int> s, vector<int> path, bool* visited, int max_depth, int depth, bool qIndex) {
    if (!visited[node]) {
        path.push_back(node);
        visited[node] = true;
        depth++;
    }

    if (!child(node, visited) || depth >= max_depth) {
        insert(path, initial, qIndex);
        initial = depth;
        path.pop_back();
        visited[int(node)] = false;
        if (depth >= max_depth) {
            depth--;
            return;
        }
    }

    for (auto nbr : m[node]->neighbors) {
        if (!visited[int(nbr)]) {
            DFSHelper(nbr, s, path, visited, max_depth, depth, qIndex);
            visited[int(nbr)] = false;
            initial--;
        }
    }
    return;
}

vector<int> HGraph::DFSHelper(int node, bool* visited, int max_depth, int depth, bool qIndex) {
    vector<int> path;
    stack<int> s;
    s.push(int(node));
    while (!s.empty())
    {
        if (depth >= max_depth + 1) {
            return path;
        }
        node = int(s.top());
        s.pop();
        if (!visited[node]) {
            path.push_back(node);
            visited[node] = true;
            for (auto nbr : m[node]->neighbors) {
                if (!visited[int(nbr)]) {
                    s.push(int(nbr));
                }
            }
            depth += 1;
        }
    }
    return path;
    
}

void HGraph::DFS(int source, int max_depth, bool qIndex) {
    bool* visited = new bool[m.size()]{ 0 };
    stack<int> s;
    return DFSHelper(source, s, path, visited, max_depth, 0, qIndex);
}

void HGraph::callDFS(int source, int max_depth, bool qIndex) {
    initial = 0;
    path.clear();
    DFS(source, max_depth, qIndex);
}

vector<int> HGraph::generatelabeledpath(vector<int> idpath) {
    vector<int> labeledpath;
    for (auto n : idpath) {
        labeledpath.push_back(m[n]->label);
    }
    return labeledpath;
}

void HGraph::insert(vector<int> idpath, int initial, bool qIndex) {
    vector<int> labeledpath = generatelabeledpath(idpath);
    if (qIndex) {
        t.insertOne(labeledpath, t.root, idpath[0], initial);
    }
    else {
        t.insertAll(labeledpath, t.root, idpath[0], initial);
    }
}
