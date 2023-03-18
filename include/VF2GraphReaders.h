#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
//#include "LabelMap.h"
#include "HGraph.h"
#include "GGSXVFLib.h"

using namespace std;
/*
 * Abstract class
 */
class VF2GraphReader{
public:
	//LabelMap& _labelMap;
	std::map<std::string, int> _labelMap;
	//VF2GraphReader(LabelMap& labelMap) : _labelMap(labelMap){}
	VF2GraphReader(std::map<std::string, int> labelMap) : _labelMap(labelMap){}
	/*
	 * Read next graph.
	 */
	virtual GGSXVFLib::MstlARGraph* readGraph() = 0;
	/*
	 * Stop stream pointer before graph.
	 * Just go forward needed.
	 */
	virtual bool gotoGraph(int id) =0;
};


/*
 * Default format implementation.
 * Undirect graphs.
 */
class VF2GraphReader_gff : public VF2GraphReader{
public:
	bool check=true;
	enum State {
		AttendGraph,
		LoadNumNodes,
		LoadNodes,
		LoadNumEdges,
		LoadEdges,
		End
	};
	enum EState{
		FirstEdge,
		SecondEdge
	};


	std::ifstream& in;
	int c_id;

	VF2GraphReader_gff(std::map<std::string, int> labelMap, std::ifstream& _in)
			: VF2GraphReader(labelMap), in(_in){
		c_id = 0;
	}

	virtual bool gotoGraph(int id){
//		std::cout<<"go("<<id<<"):"<<c_id<<"\n";

	if(!in.is_open() || in.eof() || in.bad())
			return false;

	State state = AttendGraph;
	EState estate=FirstEdge;

	std::string s;
	int itemcount=0, i=0;
	int firste, seconde;

	while(c_id<id && !in.eof() && state!=End){
		std::cout<<"Graph id "<<c_id<<endl;
		if(check){
			in>>s;
		}
		in>>s;

		if(s=="")
			return false;

		if(s[0]=='#'){

			in>>s;
			std::cout<<"t # "<<s<<endl;
		}
		else{
			return false;
		}


		in>>s;
		if(s=="")
			return false;

		while(s[0]=='v'){
			in>>s; //vid
			if(s=="")
				return false;
			in>>s; //vlabel
			in>>s; //v
		}

		while(s[0]=='e'){
			in>>s; //firste
			if(s=="")
				return false;

			in>>s; //seconde
			if(s=="")
				return NULL;

			in>>s; //label
			in>>s; //e
		}
		if(s[0]=='t'){
			check=false;
		}
		c_id++;
	}

	if(c_id==id && !in.eof())
			return true;
		return false;
	}



	virtual GGSXVFLib::MstlARGraph* readGraph(){
	if(!in.is_open() || in.eof() || in.bad())
			return NULL;

		State state = AttendGraph;
		EState estate=FirstEdge;

		std::string s;
		int itemcount=0, i=0;
		int firste, seconde;

		ARGEdit ed;

		while(!in.eof() && state!=End){
			if(check){
				in>>s;
			}
			in>>s;

//			std::cout<<"read: "<<s<<"\n";

			if(s=="")
				return NULL;

			if(s[0]=='#'){
//				g->name = s;
				in>>s;
				std::cout<<"t # "<<s<<endl;
			}
			else{
				return NULL;
			}


			in>>s;
			if(s=="")
				return NULL;

			while(s[0]=='v'){
				std::cout<<s;
				int label_id=0;
//				g->insertNode(_labelMap.getLabel(s));
//				mstl_node_label_t* l = new mstl_node_label_t;
//				*l = _labelMap.getLabel(s);

				in>>s;
				std::cout<<" "<<s;
				in>>s;
				//ed.InsertNode( new mstl_node_label_t(_labelMap.getLabel(s)) );
				std::map<std::string, int>::iterator iter = _labelMap.find(s);
				if (iter == _labelMap.end()) {
					label_id = _labelMap.size();
					_labelMap[s] = label_id;
				}
				else {
					label_id = iter->second;
				}
				std::cout<<" "<<s<<endl;
				ed.InsertNode(&label_id);
				in>>s;
			}

			
			while(s[0]=='e'){
				std::cout<<s;
				in>>s;
				std::cout<<" "<<s;
				if(s=="")
					return NULL;
					
				//int (s[2]); 
				firste=atoi(s.c_str());

				in>>s;
				std::cout<<" "<<s<<endl;
				if(s=="")
					return NULL;
				seconde=atoi(s.c_str());

//				g->insertEdge(firste, seconde);
//				g->insertEdge(seconde, firste);
				ed.InsertEdge(firste, seconde, NULL);
				ed.InsertEdge(seconde, firste, NULL);
				in>>s;
				in>>s;
			}
			if(s[0]=='t'){
				check=false;
			}
			state=End;
		}


		c_id++;
		return new GGSXVFLib::MstlARGraph(&ed);;
	}
};


