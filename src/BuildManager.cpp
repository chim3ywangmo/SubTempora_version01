#include "../include/BuildManager.h"

vector<uint32_t> BuildManager::union_vec(vector<uint32_t> vec1, vector<uint32_t> vec2){
    vector<uint32_t> merged_vec;
    std::merge(vec1.begin(), vec1.end(),
                vec2.begin(), vec2.end(),
                std::back_inserter(merged_vec));
    vector<uint32_t>::iterator pte = std::unique(merged_vec.begin(), merged_vec.end());
    merged_vec.erase(pte, merged_vec.end());
    return merged_vec;
}
void BuildManager::write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<double>>> dataset) {
    std::ofstream myFile(filename, fstream::app);
    std::ifstream ifs(filename, std::ios::ate);
    if (ifs.tellg() == 0) {
        for (int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).first;
            if (j != dataset.size() - 1) myFile << ","; 
        }
        myFile << "\n";
    }
    for (int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for (int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if (j != dataset.size() - 1) myFile << ","; 
        }
        myFile << "\n";
    }
    myFile.close();
}
void BuildManager::BuildGraphIndex(std::string file, std::string queryName, int maxLength, int maxGID, bool compress, int option, int datasetTyp) {
    GraphReader gr(file, gIndex, maxLength, maxGID, false);
    _labelMap.insert(gr._labelMap.begin(), gr._labelMap.end());
    string path;
    if (option == 0)
    {
        path = "Results/HRTree/g" + std::to_string(maxGID) + "HRTree" + std::to_string(maxLength)+queryName+".txt";
    }
    else if (option == 1)
    {
        path = "Results/HRTreeIVSIMDPFor/g" + std::to_string(maxGID) + "HRTreeIV" + std::to_string(maxLength) +queryName+".txt";
    }
    else if (option == 2)
    {
        path = "Results/HRTreeElias/g" + std::to_string(maxGID) + "HRTreeIVElias" + std::to_string(maxLength) +queryName+".txt";
    }
    else if (option == 3) {
        path = "Results/HRTreeIVSimple9/g" + std::to_string(maxGID) + "HRTreeIVSimple9" + std::to_string(maxLength) +queryName+".txt";
    }
    else if (option == 4) {
        path = "Results/HRTreeBMEWAH/g" + std::to_string(maxGID) + "HRTreeBMEWAH" + std::to_string(maxLength) +queryName+".txt";
    }

    vector<double>times;
    switch (option) {
    case 0: {
        times = gIndexIV.run(compress, &gIndex, option);
        break;
    }
    case 1: case 4: {
        times = gIndexIV.run(compress, &gIndex, option);
        break;
    }
    case 2:
    {
        break;
    }
    case 3:
    {
        times = gIndexIV.run(compress, &gIndex, option);
        break;
    }
    }

    cout << "Elapsed second (sec)" << gr.time << endl;
    myfile.open(path, std::ios::out | std::ios::binary);
    gr.writeHRTree(myfile, gIndex, option);
    myfile.flush();
    myfile.close();

    ifstream in_file(path, std::ios::in | ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    type.push_back(option);
    datasetType.push_back(datasetTyp);
    constructionTime.push_back(gr.time);
    constructionSize.push_back(file_size);
    in_file.close();
    compressionTime.push_back(times[0]);
    maxSize.push_back(maxGID - 1);
    maxLen.push_back(maxLength);
    std::vector<std::pair<std::string, std::vector<double>>> vals = { {"Type", type}, {"Dataset", datasetType}, {"ConstructionTime", constructionTime}, {"CompressionTime", compressionTime}, {"ConstructionSize", constructionSize}, {"MaxSize", maxSize}, {"MaxLength", maxLen} };
    write_csv("Results/BenchmarkResultvaryG", vals);
    type.clear();
    datasetType.clear();
    constructionTime.clear();
    constructionSize.clear();
    compressionTime.clear();
    maxSize.clear();
    maxLen.clear();
}

void BuildManager::BuildQueryIndex(std::string file, std::string queryName, int maxLength, int maxGID, bool compress, int option, int datasetTyp) {
    GraphReader gr(file, qIndex, maxLength, maxGID, true);
    string path;
    if (option == 0)
    {
        path = "Results/HRTree/q" + std::to_string(maxGID) + "HRTree" + std::to_string(maxLength) +queryName+ ".txt";
    }
    else if (option == 1)
    {
        path = "Results/HRTreeIVSIMDPFor/q" + std::to_string(maxGID) + "HRTreeIV" + std::to_string(maxLength) +queryName+ ".txt";
    }
    else if (option == 2)
    {
        path = "Results/HRTreeElias/q" + std::to_string(maxGID) + "HRTreeIVElias" + std::to_string(maxLength) +queryName+  ".txt";
    }
    else if (option == 3) {
        path = "Results/HRTreeIVSimple9/q" + std::to_string(maxGID) + "HRTreeIVSimple9" + std::to_string(maxLength) +queryName+  ".txt";
    }
    else if (option == 4) {
        path = "Results/HRTreeBMEWAH/q" + std::to_string(maxGID) + "HRTreeBMEWAH" + std::to_string(maxLength) +queryName+  ".txt";
    }

    vector<double>times;
    switch (option) {
    case 0: {
        times = gIndexIV.run(compress, &qIndex, option);
        break;
    }
    case 1: case 4: {
        times = gIndexIV.run(compress, &qIndex, option);
        break;
    }
    case 2:
    {
        break;
    }
    case 3:
    {
        times = gIndexIV.run(compress, &qIndex, option);
        break;
    }
    }

    std::cout << "\n";
    cout << "Elapsed second (ms)" << gr.time << endl;
    myfile.open(path, std::ios::out);
    gr.writeHRTree(myfile, qIndex, option);
    myfile.flush();
    myfile.close();

    ifstream in_file(path, ios::binary);
    in_file.seekg(0, ios::end);
    int file_size = in_file.tellg();
    cout << "Size of the file is" << " " << file_size << " " << "bytes";
    type.push_back(option);
    datasetType.push_back(datasetTyp);
    constructionTime.push_back(gr.time);
    if(option==1){
        constructionSize.push_back(file_size+gIndexIV.compressedSizeIV);
    }
    else{
        constructionSize.push_back(file_size);
    }
    in_file.close();
    compressionTime.push_back(times[0]);
    maxSize.push_back(maxGID - 1);
    maxLen.push_back(maxLength);
    std::vector<std::pair<std::string, std::vector<double>>> vals = { {"Type", type}, {"Dataset", datasetType}, {"ConstructionTime", constructionTime}, {"CompressionTime", compressionTime}, {"ConstructionSize", constructionSize}, {"MaxSize", maxSize}, {"MaxLength", maxLen} };
    write_csv("Results/BenchmarkResultQueryvaryG", vals);
    type.clear();
    datasetType.clear();
    constructionTime.clear();
    constructionSize.clear();
    compressionTime.clear();
    maxSize.clear();
    maxLen.clear();

}

std::vector<int> BuildManager::findLongestCommonPrefix(std::vector<int> s1, std::vector<int> s2) {
    int minLength = std::min(s1.size(), s2.size());
    for (int i = 0; i < minLength; i++) {
        if (s1[i] != s2[i]) {
            std::vector<int> result(i - 0);
            copy(s2.begin() + 0, s2.begin() + i, result.begin());
            return result;
        }
    }
    std::vector<int> result(minLength);
    if (minLength < s2.size()) {
        copy(s2.begin() + 0, s2.begin() + minLength, result.begin());
    }
    else {
        copy(s2.begin() + 0, s2.begin() + s2.size(), result.begin());
    }
    return result;
}
std::vector<int> BuildManager::vectorSlice(int x, int y, std::vector<int> s1) {
    if (y < x) {
        return {};
    }
    std::vector<int> result(y - x);
    copy(s1.begin() + x, s1.begin() + y, result.begin());
    return result;
}

void BuildManager::match(int option) {
    HRNode* qNode; 
    HRNode* gNode; 
    HRNode* child_qNode = NULL;
    HRNode* child_gNode = NULL;
    HRNode* old_gNode;
    QueueNode* current_qn;
    QueueNode* tail_qn;
    QueueNode* delete_qn;
    vector<int> qlabel;
    vector<int> commonPrefix;
    std::unordered_map<int, HRNode*>::iterator node;
    qNode = this->qIndex.root;
    gNode = this->gIndex.root;
    current_qn = new QueueNode(qNode, gNode, qlabel, false, 0);
    tail_qn = current_qn;
    bool check = true;

    while (current_qn != NULL)
    {
        qNode = current_qn->fNode;
        gNode = current_qn->sNode;
        qlabel = current_qn->qlabel; 
        if (!qlabel.empty()) { 
            child_qNode = qNode;
            child_gNode = gNode->children[qlabel[0]];
        }
        else { 
            node = qNode->children.begin();
            if (node != qNode->children.end()) {
                child_qNode = node->second;
                child_gNode = gNode->children[child_qNode->label[0]];
                qlabel = child_qNode->label;
            }
            else {
                child_qNode = NULL;
            }
        }

        if (qNode->leaf && qlabel.empty()) {
            check = matched_nodes(qNode, gNode, current_qn->pos, option);
        }
        old_gNode = child_gNode;

        while (child_qNode != NULL && check)
        {

            if (child_gNode == NULL || child_gNode->label[0] != qlabel[0]) {
                check = false;
            }
            else {
                commonPrefix = findLongestCommonPrefix(qlabel, child_gNode->label);
                qlabel = vectorSlice(commonPrefix.size(), qlabel.size(), qlabel);
                int position;
                if (commonPrefix.empty()) {
                    position = 0;
                }
                else {
                    position = commonPrefix.size() - 1;
                }

                bool visit;
                if (qlabel.size() != NULL) {
                    visit = true;
                }
                else {
                    visit = false;
                }
                tail_qn->next = new QueueNode(child_qNode, child_gNode, qlabel, visit, position);
                tail_qn = tail_qn->next;
            }

            if (!current_qn->visited) {
                node++;
                if (node == qNode->children.end()) {
                    child_qNode = NULL;
                }
                else {
                    child_qNode = node->second;
                }

                if (child_qNode != NULL) {
                    qlabel = child_qNode->label;
                }
                else {
                    qlabel.push_back(0);
                }
                child_gNode = gNode->children[qlabel[0]];

            }
            else {
                break; 
            }

        }
        delete_qn = current_qn;
        current_qn = current_qn->next;
        delete delete_qn;
    }
}

bool BuildManager::prune_rule_1(HRNode* qNode, HRNode* gNode, int position, int option) {
    int ini_size = candidateGraphSet.size();
    int occ = qNode->graphInfo[0][qNode->label.size() - 1];

    switch (option) {
    case 0:
    {
        for (auto IT = gNode->graphInfo.begin(); IT != gNode->graphInfo.end(); IT++) {
            if (IT->second[position] >= occ) {
                candidateGraphSet.insert(IT->first);
            }
        }
        break;
    }
    case 1:
    { 
        std::vector<uint32_t> HRTreeSetCS;
        std::map<int, std::vector<std::vector<uint32_t>>>::const_iterator IT;
        if (gNode->graphInfoIV.find(occ) != gNode->graphInfoIV.end()) {
            IT = gNode->graphInfoIV.find(occ);
            vector<uint32_t> mydataback(gNode->HRTreeorigSize[IT->first][position]); 
            size_t recoveredsize = mydataback.size();
            using namespace SIMDCompressionLib;
            IntegerCODEC &codec = *CODECFactory::getFromName("s4-fastpfor-d1");
            codec.decodeArray(IT->second[position].data(), IT->second[position].size(), mydataback.data(), recoveredsize);
            mydataback.resize(recoveredsize);
            HRTreeSetCS= mydataback;
        }
        candidateGraphSetIV = HRTreeSetCS;
        break;

    }
    case 2:
    {
        for (auto IT = gNode->graphInfoIV.find(occ); IT != gNode->graphInfoIV.end(); IT++) {
            for (auto val : IT->second[position]) {
                candidateGraphSet.insert(val);
            }
        }
        break;
    }
    case 4:
    {
        typedef ewah::EWAHBoolArray<uint32_t> bitmap;
        bitmap bitsetCS;
        std::map<int, vector<bitmap>>::const_iterator IT;
        if (gNode->graphInfoBM.find(occ) != gNode->graphInfoBM.end()) {
            IT = gNode->graphInfoBM.find(occ);
            bitsetCS = IT->second[position];
        }
        candidateGraphSet = toSet(bitsetCS);
        break;
    }
    }

    return ini_size != candidateGraphSet.size();
}
bool BuildManager::prune_rule_1(HRNode* qNode, HRNode* gNode, int position, std::set<uint32_t>::iterator cgs_IT) { 
    auto gis_IT = gNode->graphInfo.find(*cgs_IT);
    if (gis_IT == gNode->graphInfo.end()) {
        return false;
    }
    else {
        if (gis_IT->second[position] < qNode->graphInfo[0][qNode->label.size() - 1]) {
            return false;
        }
    }
    return true;
}

void BuildManager::prune_rule_2(HRNode* qNode, HRNode* gNode, int position) { 
    using namespace SIMDCompressionLib;
    IntegerCODEC &codec = *CODECFactory::getFromName("s4-fastpfor-d1");
    auto firstIV = qNode->graphInfoIV.begin();
    int occ = firstIV->first;
    std::vector<uint32_t> HRTreeSetCS=candidateGraphSetIV;
    std::vector<uint32_t>  tempIVsetCS(HRTreeSetCS.size()); 
    std::map<int, std::vector<std::vector<uint32_t>>>::const_iterator IT;
    intersectionfunction inter =
    IntersectionFactory::getFromName("simd"); 
    if (gNode->graphInfoIV.find(occ) != gNode->graphInfoIV.end()) {
        IT = gNode->graphInfoIV.find(occ);
        vector<uint32_t> mydataback2(gNode->HRTreeorigSize[IT->first][position]);
        size_t recoveredsize = mydataback2.size();
        codec.decodeArray(IT->second[position].data(), IT->second[position].size(), mydataback2.data(), recoveredsize);
        mydataback2.resize(recoveredsize);
        size_t intersize;
        if(HRTreeSetCS.size()>mydataback2.size()){
            tempIVsetCS.resize(mydataback2.size());
            intersize= inter(HRTreeSetCS.data(), HRTreeSetCS.size(), mydataback2.data(), mydataback2.size(), tempIVsetCS.data());
        }
        else{
            intersize= inter(HRTreeSetCS.data(), HRTreeSetCS.size(), mydataback2.data(), mydataback2.size(), tempIVsetCS.data());
        }
        tempIVsetCS.resize(intersize);
        tempIVsetCS.shrink_to_fit();
    }
    else {
        tempIVsetCS=HRTreeSetCS;
    }
    candidateGraphSetIV = tempIVsetCS;
}

void BuildManager::prune_rule_3(HRNode* qNode, HRNode* gNode, int position) { 
    auto firstIV = qNode->graphInfoBM.begin();
    int occ = firstIV->first;
    typedef ewah::EWAHBoolArray<uint32_t> bitmap;
    bitmap bitsetCS;
    bitmap tempBitsetCS, tempBitsetCS2; 
    for (auto i : candidateGraphSet) {
        bitsetCS.set(i);
    }
    std::map<int, vector<bitmap>>::const_iterator IT;
    if (gNode->graphInfoBM.find(occ) != gNode->graphInfoBM.end()) {
        IT = gNode->graphInfoBM.find(occ);
        tempBitsetCS = bitsetCS & IT->second[position];
    }
    else {
        tempBitsetCS = bitsetCS;
    }
    tempBitsetCS2 = tempBitsetCS;
    bitsetCS = tempBitsetCS2;
    candidateGraphSet = toSet(bitsetCS);

}
bool BuildManager::matched_nodes(HRNode* qNode, HRNode* gNode, int position, int option) {
    if (second) {
        prune_rule_1(qNode, gNode, position, option);
        second = false;
    }
    else {
        switch (option) {
        case 0: { 
            auto cgs_IT = candidateGraphSet.begin();
            bool erase = false;

            while (cgs_IT != candidateGraphSet.end()) {
                erase = !prune_rule_1(qNode, gNode, position, cgs_IT);
                if (erase) {
                    candidateGraphSet.erase(cgs_IT++);
                }
                else {
                    cgs_IT++;
                }
            }
            break;
        }
        case 1:
        {
            prune_rule_2(qNode, gNode, position);
            break;
        }
        case 4: 
        {
            prune_rule_3(qNode, gNode, position);
            break;
        }
        }
    }

    if(option==0 || option==4){
        if (candidateGraphSet.size() == 0) {
            return false;
        }
    }
    else if(option=1){
        if (candidateGraphSetIV.size()==0) {
            return false;
        }
    }

    return true;
}
