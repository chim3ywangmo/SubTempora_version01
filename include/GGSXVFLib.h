/*
 * GGSXVFLib.h
 *
 *  Wrappers for VFLib
 */
#pragma once
#include <map>
#include <vf2lib/argraph.h>
#include <vf2lib/argedit.h>
#include <vf2lib/vf2_mono_state.h>
#include <vf2lib/vf2_sub_state.h>
#include <vf2lib/match.h>

/*
 * VFLib graph class.
 */
namespace GGSXVFLib{
	class MstlARGraph : public ARGraph<int, void>{
	public:
		MstlARGraph(ARGEdit* ed) : ARGraph<int,void>(ed){};
	};

	/*
	* Convert HRTree Graph Type into VFLib graph type.
	*/
	MstlARGraph* convertGraf(HGraph& g){
		ARGEdit ed;
		for(auto IT=g.m.begin(); IT!=g.m.end(); IT++){
				ed.InsertNode(&(IT->second->label));
		}

		for(auto IT=g.m.begin();
				IT!=g.m.end(); IT++){
			for(auto ITT=IT->second->neighbors.begin();
					ITT!=IT->second->neighbors.end(); ITT++){
				ed.InsertEdge(IT->first,*ITT,NULL);
			}
		}
		MstlARGraph* vg=new MstlARGraph(&ed);
		return vg;
	};

	/*
	* Label comparator.
	*/
	class VIntegerComparator: public AttrComparator{
	public:
		virtual bool compatible(void* pa, void* pb){
			return (*((int*)pa))==(*((int*)pb));
		};
	};

	/*
	* Match. Monomorphism.
	* g1 must be the query.
	* g2 must be the reference.
	*/
	void matchvf_monostate(	MstlARGraph* g1,
							MstlARGraph* g2,
							match_visitor visitor,
							void* data){

		VF2MonoState s0(g1,g2);
		//VF2SubState s0(g1,g2);
		match(&s0, visitor, data);
	};

}
