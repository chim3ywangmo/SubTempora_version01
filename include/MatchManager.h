#pragma once
#include "../include/HGraph.h"
#include "../include/GGSXVFLib.h"
#include "../include/Timer.h"
#include <map>
#include <list>

class my_visitor_data_t{
public:
	std::ostream& os;
	int graph_id;
	int query_id;
	bool justFirst;
	int match_count;

	my_visitor_data_t(std::ostream& _os, bool _justFirst)
		: os(_os), justFirst(_justFirst){
		graph_id=-1;
		query_id = -1;
		match_count = 0;
	}
};

/*
 * Print match on std::ostream
 */
bool my_visitor(int n, node_id ni1[], node_id ni2[], void* usr_data){
	my_visitor_data_t* vis = (my_visitor_data_t*)usr_data;
	vis->match_count++;
	vis->os<<vis->query_id<<":"<<vis->graph_id<<":";
	vis->os<<"{";
	for(int i=0;i<n;i++){
		vis->os<<"("<<ni1[i]<<","<<ni2[i]<<")";
		if(i!=n-1)
			vis->os<<",";
	}
	vis->os<<"}\n";
	return vis->justFirst;
};

/*
 * Jump match printing
 */
bool my_visitor_null(int n, node_id ni1[], node_id ni2[], void* usr_data){
	my_visitor_data_t* vis = (my_visitor_data_t*)usr_data;
	vis->match_count++;
	return vis->justFirst;
};


class MatchManager{

public:
	GGSXVFLib::MstlARGraph* _query;
	//GraphReader& 			_greader;
	std::map<int, GGSXVFLib::MstlARGraph* > _dbgraphs;
	my_visitor_data_t* 		_visitor;
	bool 					_printMatches;
	bool					_full_verbose;
	double					_pure_match_time;


	MatchManager(int query_id, GGSXVFLib::MstlARGraph* query, std::map<int, GGSXVFLib::MstlARGraph* > dbgraphs, std::ostream& os, bool printMatches, bool justFirst, bool full_verbose)
			: _dbgraphs(dbgraphs){
		_query = query;
//		_query->SetNodeComparator(new GGSXVFLib::VIntegerComparator());
		_visitor = new my_visitor_data_t(os, justFirst);
		_visitor->query_id = query_id;
		_printMatches = printMatches;
		_full_verbose = full_verbose;
		_pure_match_time = 0;
	}

	~MatchManager(){
		delete _query;
	}




	void run(std::set<uint32_t> gset){
		Timer t;
		std::cout<<"Matching candidate graphs\n";
		for(std::set<uint32_t>::iterator IT = gset.begin(); IT!=gset.end(); IT++){
			std::cout<<"Candidate "<<*IT<<"...\n ";

//			if(_greader.gotoGraph(*IT)){
			if(_dbgraphs.find(*IT) != _dbgraphs.end()){
				GGSXVFLib::MstlARGraph* vga= _dbgraphs.find(*IT)->second;
//				if(_greader.readGraph(g)){

//					GGSXVFLib::MstlARGraph* vga = GGSXVFLib::convertGraf(*g);
					_visitor->graph_id = *IT;

					t.start();

					GGSXVFLib::matchvf_monostate(_query, vga, my_visitor, _visitor);
					//GGSXVFLib::matchvf_monostate(_query, vga, my_visitor_null, _visitor);
					t.stop();
					_pure_match_time += t.elapsedMilliseconds();

					if(_full_verbose) std::cout<<"done\n";

//					delete vga;
//					delete g;
//				}else{
//					if(_full_verbose) std::cout<<"error on reading graph from file!\n";
//				}
			}else{
				std::cout<<"error db graph not loaded!\n";
			}
		}
	}
};

