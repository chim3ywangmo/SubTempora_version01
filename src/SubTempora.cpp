#include "../include/SubTempora.h"
#include <stdlib.h>
void printVec(string line, vector<double> vec){
    std::cout<<"\n";
    std::cout<<line;
    for(auto n: vec){
        std::cout<<n<<", ";
    }
    std::cout<<"\n";
}

void printVec(string line, vector<uint32_t> vec){
    std::cout<<"\n";
    std::cout<<line;
    for(auto n: vec){
        std::cout<<n<<", ";
    }
    std::cout<<"\n";
}

void printSet(string line, std::set<uint32_t> vec){
    std::cout<<"\n";
    std::cout<<line;
    for(auto n: vec){
        std::cout<<n<<",";
    }
    std::cout<<"\n";
}

void load_graphsdb(const std::string& db_file, std::map<std::string, int> labelMap, std::set<uint32_t> &candidateGraphSet, std::map<int, GGSXVFLib::MstlARGraph*>& graphsdb){
    std::ifstream dbis;
    dbis.open(db_file, std::ios::in);
    VF2GraphReader_gff dbgreader(labelMap, dbis);
    for(auto IT = candidateGraphSet.begin(); IT!=candidateGraphSet.end(); IT++){
        dbgreader.gotoGraph(*IT);
        GGSXVFLib::MstlARGraph* g;
        if((g = dbgreader.readGraph()) != NULL){
            graphsdb.insert(std::pair<int, GGSXVFLib::MstlARGraph*>(*IT, g));
        }
        else{
            std::cout<<"error on reading graph from file!\n";
        }
    }

    dbis.close();
}
void load_graphsdb(const std::string& db_file, std::map<std::string, int> labelMap,std::map<int, GGSXVFLib::MstlARGraph*>& graphsdb){
    std::ifstream dbis;
	dbis.open(db_file, std::ios::in);
	VF2GraphReader_gff dbgreader(labelMap, dbis);

	int c_graph_id = 0;
	bool readed = false;

	do{
		std::cout<<"Trying to read graph "<<c_graph_id<<"...";

		GGSXVFLib::MstlARGraph* g;
		readed = (g = dbgreader.readGraph()) !=NULL;

		if(readed){
			graphsdb.insert(std::pair<int, GGSXVFLib::MstlARGraph*>(c_graph_id, g));

			c_graph_id++;
		}
		else{
			std::cout<<" failed! ...may be the last?\n";
		}
	}while(readed);

	dbis.close();
}

void match_query(int query_id, GGSXVFLib::MstlARGraph* query_graph, std::map<int, GGSXVFLib::MstlARGraph* > dbgraphs, std::ostream& os, std::set<uint32_t> &candidateGraphSet, int* mcount, double* pure_time){
    MatchManager mman(query_id, query_graph, dbgraphs, os, true, false, true);
    std::cout<<"Matching started.."<<endl;
	mman.run(candidateGraphSet);
	*mcount =  mman._visitor->match_count;
	*pure_time = mman._pure_match_time;
}

void match_query(int query_id, GGSXVFLib::MstlARGraph* query_graph, std::map<int, GGSXVFLib::MstlARGraph* > dbgraphs, std::set<uint32_t> &candidateGraphSet, int* mcount, double* pure_time){
    std::ofstream fos;
    std::string moutput_file="moutput_file";
    fos.open(moutput_file.c_str(), std::ios::out);
    if(!fos.is_open() || fos.bad()){
        std::cout<<"Error on opening output file : "<< "moutput_file" <<" \n";
        exit(1);
    }
    match_query(query_id, query_graph, dbgraphs, fos, candidateGraphSet, mcount, pure_time);
    fos.flush();
    fos.close();
}

double find(std::string db_file, std::string qName, std::map<std::string, int> _labelMap, std::set<uint32_t> candidateGraphSet, GGSXVFLib::MstlARGraph* vf2_query_graph){
    double match_time;
 	std::map<int, GGSXVFLib::MstlARGraph*> graphsdb;
	load_graphsdb(db_file, _labelMap, candidateGraphSet, graphsdb);
    int mcount=0;
	double pure_match_time=0;
	match_query(0, vf2_query_graph, graphsdb, candidateGraphSet, &mcount, &pure_match_time);
    return pure_match_time;
}

int main() {
    //*****************************************************//
    int maxEdge=20;//--->CHANGE for varyE but fixed for varyV
    int maxVertices=200; 
    double maxP=0.0125; 
    //std::string file="SynErdosRenyiv"+std::to_string(maxVertices)+"d2e"+std::to_string(maxEdge)+"p0.0125g1000";//--for varyE
    std::string file="SynErdosRenyiv"+std::to_string(maxVertices)+"p"+std::to_string(maxP).substr(0, std::to_string(maxP).find(".") + 5)+"e"+std::to_string(maxEdge)+"g1000"; //for varyV
    //std::string file="SynErdosRenyiv"+std::to_string(maxVertices)+"p"+std::to_string(maxP).substr(0, std::to_string(maxP).find(".") + 5)+"e"+std::to_string(maxEdge)+"g1000"; //for varyp
    std::cout<<file<<endl;
    std::string file5 = "Datasets/Erdos-Renyi/varyV/"+file+".graph";
    //*****************************************************//
    std::vector<string> queryVec;
    int candidateSetSize1, candidateSetSize2, candidateSetSize3;
    Timer t;
    vector<double> filteringTime;
    vector<double> matchTime;
    vector<int> maxLength = {4}; 
    vector<int> maxGID1 = {1001}; 
    double time=0.0;
    vector<bool> compress = { true, false };
    vector<int> option = { 0,1,2,3,4 }; //option-0:HRTree, 1:HRTreeIV+FastPFor, 4:HRTreeBM+EWAH
    BuildManager bdOld,bd1,bd2;
    vector<int> datatype = { 4 }; //4:SynErdosRenyi
    int maxSize=0;
    for (auto i : maxGID1) {
    for (auto j : maxLength) {
        std::string qName=file+std::to_string(maxSize);
        //Uncomment following one by one to benchmark     
        //bdOld.BuildGraphIndex(file5, qName, j, i, compress[1], option[0], datatype[4]); //HRTree   
        //bd1.BuildGraphIndex(file5, qName, j, i, compress[0], option[1], datatype[4]); //HRTreeIV
        //bd2.BuildGraphIndex(file5, qName, j, i, compress[0], option[4], datatype[4]); //HRTreeBM
    }
    }
    return 0;
}
