#include "A2Sdf.h"

#include <mut.h>
#include <abl.h>
#include <mlo.h>
#include <mlu.h>
#include <abe.h>
#include <abv.h>

/*for inputs to output:   Tphl_i_t */
#define Tphl "tphl"
#define Tpll "tpll"
#define Tplh "tplh"
#define Tphh "tphh"

/*for flip-flop*/
#define Tsr "tsr" /*  tsr_i_ck	*/
#define Tsf "tsf" /*  tsf_i_ck	*/
#define Thr "thr" /*  thr_i_ck	*/
#define Thf "thf" /*  thf_i_ck  */
#define Tar "tar" /*  tar_ck_q  */
#define Taf "taf" /*  taf_ck_q  */


A2Sdf::~A2Sdf()
{
    for (TimingCells::iterator it = _timingCells.begin();
	    it != _timingCells.end();
	    it++)
    {
	delete *it;
    }

}

void
A2Sdf::CreateTimingCell(const char* name)
{
    char* Thl, *Tll, *Tlh, *Thh, *tar, *taf;
    TimingCell* timingCell = new TimingCell(name);
    _timingCells.push_back(timingCell);
    
    befig_list* befig = vhdlloadbefig(NULL,const_cast<char*>(name),0/*don't dump message*/);
    assert(befig);

    if (_verbose)
    {
	cout << " o Treating cell: " << name << endl;
    }

    list<const char*> outs;
    for (const bepor_list* bepor=befig->BEPOR; bepor; bepor=bepor->NEXT)
    {
	if (isvdd(bepor->NAME) || isvss(bepor->NAME)) continue;
	if (bepor->DIRECTION == OUT || bepor->DIRECTION == INOUT)
	{
	    outs.push_back(bepor->NAME);
	}
    }

    for (list<const char*>::const_iterator lit = outs.begin();
	    lit != outs.end();
	    lit++)
    {
	const char* output = *lit;
	for (const bepor_list* bepor=befig->BEPOR; bepor; bepor=bepor->NEXT)
	{
	    char* newCon = bepor->NAME;
	    if (newCon == output) continue;
	    if (isvdd(newCon) || isvss(newCon)) continue;
	    if (bepor->DIRECTION == IN || bepor->DIRECTION == INOUT)
	    {
		char memo=SEPAR; 
		SEPAR='_';
		Thl=concatname(Tphl,newCon);
		Tll=concatname(Tpll,newCon);
		Thh=concatname(Tphh,newCon);
		Tlh=concatname(Tplh,newCon);
		tar=concatname(Tar,newCon);
		taf=concatname(Taf,newCon);
		Thl=concatname(Thl,output);
		Tll=concatname(Tll,output);
		Thh=concatname(Thh,output);
		Tlh=concatname(Tlh,output);
		tar=concatname(tar,output);
		taf=concatname(taf,output);
		SEPAR=memo;

		for (const begen_list* begen=befig->BEGEN; begen; begen=begen->NEXT)
		{
		    if (begen->NAME==Tlh || begen->NAME==Thh)
		    {
			IOPath* ioPath = timingCell->AddIOPath(newCon, output);
			ioPath->CreateRisingPath(*(int*)begen->VALUE);
		    }
		    if (begen->NAME==Thl || begen->NAME==Tll)
		    {
			IOPath* ioPath = timingCell->AddIOPath(newCon, output);
			ioPath->CreateFallingPath(*(int*)begen->VALUE);
		    }
		}
	    }
	}
    }
}

void
A2Sdf::Dump() const
{
    ofstream out(_fileName);
    out << "(DELAYFILE" << endl
	<< "  (SDFVERSION \"3.0\")" << endl
	<< "  (VENDOR     \"ASIM/LIP6\")" << endl
	<< "  (VERSION    \"1.0b\")" << endl
	<< "  (TIMESCALE  1 ps)" << endl;
    for (TimingCells::const_iterator it = _timingCells.begin();
	    it != _timingCells.end();
	    it++)
    {
	(*it)->Dump(out);
    }
    out << ")" << endl;
}
