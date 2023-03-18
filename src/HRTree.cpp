#include "../include/HRTree.h"

    void HRTree::setGraphInfo(map<int, vector<vector<uint32_t>>> graphInfoIV, HRNode* currentNode) {
        currentNode->graphInfoIV = graphInfoIV;
    }
    void HRTree::incrementCountOne(int vid, std::vector<int>s, HRNode* currentNode, HRNode* node, int initial, bool childExists) {
        if (node->children[s[0]]->graphInfo[gid].empty()) { 
            node->children[s[0]]->vertexid = vid;
            std::vector<int> arr(s.size());
            std::fill(arr.begin(), arr.end()-1, 0);
            arr[s.size() - 1] = 1;
            node->children[s[0]]->graphInfo[gid].resize(arr.size());
            node->children[s[0]]->graphInfo[gid] = arr;
        }
        else {
            std::vector<int> count;
            if (childExists) {
                for (auto n : node->children[s[0]]->graphInfo[gid]) {
                    count.push_back(NULL);
                }
            }
            else {
                for (auto n : node->children[s[0]]->graphInfo[gid]) {
                    count.push_back(n + 1);
                }

                std::fill(count.begin(), count.end() - 1, 0);
            }

            int sSize = s.size();
            if (node->children[s[0]]->graphInfo[gid].size() < s.size()) {
                sSize = node->children[s[0]]->graphInfo[gid].size();
            }
            for (int i = initial; i < sSize; i++) {
                node->children[s[0]]->graphInfo[gid][i] = count[i];
            }
        }
        start = false;
    }

    void HRTree::incrementCount(int vid, std::vector<int>s, HRNode* currentNode, HRNode* node, int initial) {
        if (node->children[s[0]]->graphInfo[gid].empty()) { 
            node->children[s[0]]->vertexid = vid;
            std::vector<int> arr(s.size());
            std::fill(arr.begin(), arr.end(), 1);
            node->children[s[0]]->graphInfo[gid].resize(arr.size());
            node->children[s[0]]->graphInfo[gid] = arr;
        }
        else {
            std::vector<int> count;
            for (auto n : node->children[s[0]]->graphInfo[gid]) {
                count.push_back(n + 1);
            }

            int sSize = s.size();
            if (node->children[s[0]]->graphInfo[gid].size() < s.size()) {
                sSize = node->children[s[0]]->graphInfo[gid].size();
            }
            for (int i = initial; i < sSize; i++) {
                node->children[s[0]]->graphInfo[gid][i] = count[i];
            }
        }
        start = false;
    }

    void HRTree::insertOne(std::vector<int> s, HRNode* node, int vid, int initial) {
        if (s.empty()) { 
            node->leaf = true;
        }
        else {
            if (!node->children[s[0]]) { 
                node->children[s[0]] = new HRNode(s, true, gid, vid);
                std::vector<int> count; 
                for (auto n = 0; n < node->children[s[0]]->graphInfo[gid].size(); n++) {
                    if (n==node->children[s[0]]->graphInfo[gid].size()-1) {
                        count.push_back(node->children[s[0]]->graphInfo[gid][n]+1);
                    }
                    else {
                        count.push_back(NULL);
                    }
                }
                node->children[s[0]]->graphInfo[gid] = count;
                start = true;
            }
            else {
                std::vector<int> commonPrefix = findLongestCommonPrefix(s, node->children[s[0]]->label);
                HRNode* currentNode = node->children[s[0]];
                std::vector<int> nodeSuffix = vectorSlice(commonPrefix.size(), currentNode->label.size(), currentNode->label);
                std::vector<int> sSuffix = vectorSlice(commonPrefix.size(), s.size(), s);
                if (nodeSuffix.empty() && sSuffix.empty()) { 
                    incrementCountOne(vid, s, currentNode, node, initial, false);
                    start = true;
                    node->leaf = true;
                    return;
                }
                else if (nodeSuffix.empty()) {
                    incrementCountOne(vid, commonPrefix, currentNode, node, initial, true);
                    if (initial <= commonPrefix.size()) {
                        initial = 0;
                    }
                    else {
                        initial = initial - commonPrefix.size();
                    }
                    insertOne(sSuffix, currentNode, vid, initial); 
                }
                else { 
                    std::vector<int> count;
                    int last = -1;
                    if (node->children[commonPrefix[0]]->graphInfo[gid].empty()) {
                        last = gid;
                        std::vector<int> arr(commonPrefix.size());
                        std::fill(arr.begin(), arr.end()-1, 0);
                        arr[commonPrefix.size() - 1] = 1;
                        count.resize(arr.size());
                        count = arr;
                    }
                    else {
                        for (int it = 0; it < commonPrefix.size(); it++) {
                            if (it >= initial) {
                                count.push_back(node->children[commonPrefix[0]]->graphInfo[gid][it] + 1);
                            }
                            else {
                                count.push_back(node->children[commonPrefix[0]]->graphInfo[gid][it]);
                            }
                        }

                        std::fill(count.begin(), count.end() - 1, 0);
                    }
                    node->children[commonPrefix[0]] = new HRNode(commonPrefix, false, gid, vid);
                    std::vector<int> result2;

                    if (currentNode->graphInfo.size() > 1) {                      
                        for (auto n : currentNode->graphInfo) {
                            if (n.first == last) {
                                result2.insert(result2.begin(), count.begin(), count.begin() + commonPrefix.size());
                                node->children[commonPrefix[0]]->graphInfo[n.first] = result2;
                                result2.clear();
                            }
                            else {
                                if (currentNode->graphInfo[n.first].size() < commonPrefix.size()) {
                                    result2.insert(result2.begin(), currentNode->graphInfo[n.first].begin(), currentNode->graphInfo[n.first].end());
                                }
                                else {
                                    result2.insert(result2.begin(), currentNode->graphInfo[n.first].begin(), currentNode->graphInfo[n.first].begin() + commonPrefix.size());
                                }
                                node->children[commonPrefix[0]]->graphInfo[n.first] = result2;
                                result2.clear();
                            }
                        }
                        count.clear();
                    }
                    else {
                        result2.insert(result2.begin(), count.begin(), count.begin() + commonPrefix.size());
                        node->children[commonPrefix[0]]->graphInfo[gid] = result2;
                        result2.clear();
                    }
                    currentNode->label.resize(nodeSuffix.size());
                    currentNode->label = nodeSuffix;
                    std::vector<int> result;
                    node->children[commonPrefix[0]]->children[nodeSuffix[0]] = currentNode;

                    if (currentNode->graphInfo.size() > 1) {
                        for (auto n = currentNode->graphInfo.begin(); n != currentNode->graphInfo.end();) {
                            if (n->first == last) {
                                currentNode->graphInfo.erase(last);
                                break;
                            }
                            else {
                                if (currentNode->graphInfo[n->first].size() != 0 && currentNode->graphInfo[n->first].size() > commonPrefix.size()) {
                                    result.insert(result.begin(), currentNode->graphInfo[n->first].begin() + commonPrefix.size(), currentNode->graphInfo[n->first].end());
                                    currentNode->graphInfo[n->first] = result;
                                    result.clear();
                                    n++;
                                }
                                else {
                                    n = currentNode->graphInfo.erase(n);
                                }
                            }
                        }

                    }
                    else {
                        result.insert(result.begin(), currentNode->graphInfo[gid].begin() + commonPrefix.size(), currentNode->graphInfo[gid].end());
                        currentNode->graphInfo[gid] = result;
                        result.clear();
                    }
                    if (initial <= commonPrefix.size()) {
                        initial = 0;
                    }
                    else {
                        initial = initial - commonPrefix.size();
                    }
                    insertOne(sSuffix, node->children[commonPrefix[0]], vid, initial);
                }
            }

        }
    }

    void HRTree::insertAll(std::vector<int> s, HRNode* node, int vid, int initial) {
        if (s.empty()) {
            node->leaf = true;
        }
        else {
            if (!node->children[s[0]]) { 
                node->children[s[0]] = new HRNode(s, true, gid, vid);
                std::vector<int> count; 
                for (auto n : node->children[s[0]]->graphInfo[gid]) {
                    count.push_back(n + 1);
                }
                node->children[s[0]]->graphInfo[gid] = count;
                start = true;
            }
            else {
                std::vector<int> commonPrefix = findLongestCommonPrefix(s, node->children[s[0]]->label);
                HRNode* currentNode = node->children[s[0]];
                std::vector<int> nodeSuffix = vectorSlice(commonPrefix.size(), currentNode->label.size(), currentNode->label);
                std::vector<int> sSuffix = vectorSlice(commonPrefix.size(), s.size(), s);
                if (nodeSuffix.empty() && sSuffix.empty()) {
                    incrementCount(vid, s, currentNode, node, initial);
                    start = true;
                    node->leaf = true;
                    return;
                }
                else if (nodeSuffix.empty()) {
                    incrementCount(vid, commonPrefix, currentNode, node, initial);
                    if (initial <= commonPrefix.size()) {
                        initial = 0;
                    }
                    else {
                        initial = initial-commonPrefix.size();
                    }
                    insertAll(sSuffix, currentNode, vid, initial);
                }
                else { 
                    std::vector<int> count;
                    int last=-1;
                    if(node->children[commonPrefix[0]]->graphInfo[gid].empty()){
                        last = gid;
                        std::vector<int> arr(commonPrefix.size());
                        std::fill(arr.begin(), arr.end(), 1);
                        count.resize(arr.size());
                        count=arr;
                    }
                    else {
                        for (int it = 0; it < commonPrefix.size(); it++) {
                            if (it >= initial) {
                                count.push_back(node->children[commonPrefix[0]]->graphInfo[gid][it] + 1);
                            }
                            else {
                                count.push_back(node->children[commonPrefix[0]]->graphInfo[gid][it]);
                            }
                        }
                    }
                    node->children[commonPrefix[0]] = new HRNode(commonPrefix, false, gid, vid);
                    std::vector<int> result2;

                    if (currentNode->graphInfo.size() > 1) {                     
                        for (auto n : currentNode->graphInfo) {
                            if (n.first == last) {
                                result2.insert(result2.begin(), count.begin(), count.begin()+commonPrefix.size());
                                node->children[commonPrefix[0]]->graphInfo[n.first] = result2;
                                result2.clear();
                            }
                            else {
                                if (currentNode->graphInfo[n.first].size() < commonPrefix.size()) {
                                    result2.insert(result2.begin(), currentNode->graphInfo[n.first].begin(), currentNode->graphInfo[n.first].end());
                                }
                                else {
                                    result2.insert(result2.begin(), currentNode->graphInfo[n.first].begin(), currentNode->graphInfo[n.first].begin() + commonPrefix.size());
                                }
                                node->children[commonPrefix[0]]->graphInfo[n.first] = result2;
                                result2.clear();
                            }
                        }
                        count.clear();
                    }
                    else {
                        result2.insert(result2.begin(), count.begin(), count.begin() + commonPrefix.size());
                        node->children[commonPrefix[0]]->graphInfo[gid] = result2;
                        result2.clear();
                    }
                    currentNode->label.resize(nodeSuffix.size());
                    currentNode->label = nodeSuffix;
                    std::vector<int> result;
                    node->children[commonPrefix[0]]->children[nodeSuffix[0]] = currentNode;

                    if (currentNode->graphInfo.size() > 1) {
                        for (auto n = currentNode->graphInfo.begin(); n != currentNode->graphInfo.end();) {
                            if (n->first== last) {
                                currentNode->graphInfo.erase(last);
                                break;
                            }
                            else {
                                if (currentNode->graphInfo[n->first].size()!=0 && currentNode->graphInfo[n->first].size() > commonPrefix.size()) {
                                    result.insert(result.begin(), currentNode->graphInfo[n->first].begin() + commonPrefix.size(), currentNode->graphInfo[n->first].end());
                                    currentNode->graphInfo[n->first] = result;
                                    result.clear();
                                    n++;
                                }
                                else {
                                    n=currentNode->graphInfo.erase(n);
                                }
                            }
                        }

                    }
                    else {
                        result.insert(result.begin(), currentNode->graphInfo[gid].begin() + commonPrefix.size(), currentNode->graphInfo[gid].end());
                        currentNode->graphInfo[gid] = result;
                        result.clear();
                    }
                    if (initial <=commonPrefix.size()) {
                        initial = 0;
                    }
                    else {
                        initial = initial-commonPrefix.size();
                    }
                    insertAll(sSuffix, node->children[commonPrefix[0]], vid, initial);
                }
            }

        }
    }

    bool HRTree::search(std::vector<int> s, HRNode* node) {
        if (s.empty()) {
            return node->leaf; 
        }
        else {
            if (node->children[s[0]] == NULL) {
                return false;
            }
            else {
                std::vector<int> commonPrefix = findLongestCommonPrefix(s, node->children[s[0]]->label);
                HRNode* currentNode = node->children[s[0]];
                std::vector<int> nodeSuffix = vectorSlice(commonPrefix.size(), currentNode->label.size(), currentNode->label);
                std::vector<int> sSuffix = vectorSlice(commonPrefix.size(), s.size(), s);
                if (sSuffix.empty()) {
                    return currentNode->leaf;
                }
                else if (sSuffix.empty() && nodeSuffix.empty()) {
                    return currentNode->leaf;
                }
                else if (nodeSuffix.empty()) { 
                    return search(sSuffix, currentNode);
                }
                else {
                    return false;
                }

            }
        }
    }

    std::vector<int> HRTree::vectorSlice(int x, int y, std::vector<int> s1) {
        if (y < x) {
            return {};
        }
        std::vector<int> result(y - x);
        copy(s1.begin() + x, s1.begin() + y, result.begin());
        return result;
    }

    std::vector<int> HRTree::findLongestCommonPrefix(std::vector<int> s1, std::vector<int> s2) {
        int minLength = std::min(s1.size(), s2.size());
        for (int i = 0; i < minLength; i++) {
            if (s1[i] != s2[i]) {
                std::vector<int> result(i - 0);
                copy(s2.begin() + 0, s2.begin() + i, result.begin());
                return result;
            }
        }
        std::vector<int> result(s2.size());
        copy(s2.begin() + 0, s2.begin() + s2.size(), result.begin());
        return result;
    }


