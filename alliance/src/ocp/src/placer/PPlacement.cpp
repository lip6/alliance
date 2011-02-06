
// This file is part of the Alliance Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// The Alliance Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Alliance Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Alliance Project;  if  not,  write to the  Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#include <unistd.h>
#include <string.h>
#include <set>
#include <algorithm>
#include <string>
using namespace std;

#include "mut.h"
#include "mph.h"
#include "mpu.h"
#include "mlo.h"
#include "mlu.h"

#include "iocheader.h"
#include "PMove.h"
#include "PConstants.h"
#include "PCommon.h"
#include "PPlacement.h"

struct CompPInsByWidth {
    bool operator()(PIns* Ins1, PIns* Ins2) {
	return (Ins1->GetWidth() > Ins2->GetWidth());
    }
};

PPlacement::~PPlacement()
{
    if (_verbose)
    {
	cout << " o Destruction of DATABASE ...." << endl;
    }

    for (PFixedInss::iterator fit = _fixedInss.begin();
	    fit != _fixedInss.end();
	    fit++)
    {
	delete *fit;
    }

    for (PToPlaceInss::iterator toit = _toPlaceInss.begin();
	    toit != _toPlaceInss.end();
	    toit++)
    {
	delete *toit;
    }

    for (PCons::iterator cit = _cons.begin();
	    cit != _cons.end();
	    cit++)
    {
	delete *cit;
    }

    for (PONets::iterator nit = _nets.begin();
	    nit != _nets.end();
	    nit++)
    {
	delete *nit;
    }

    for (PRows::iterator rit = _rows.begin();
	    rit != _rows.end();
	    rit++)
    {
	delete *rit;
    }

    for (PDetSubRows::iterator srit = _detSubRows.begin();
	    srit != _detSubRows.end();
	    srit++)
    {
	delete *srit;
    }
}

void
PPlacement::Init(lofig* fig, int NbRows)
{
    rflattenlofig(fig, YES, YES);
    lofigchain(fig);
    _fig = fig;
    _dx = _dy = 0;
    // construction des tables
    PNetMap pnetmap;
    PInsMap pinsmap;
    PConMap pconmap;
    PFixedMap fixedmap;
    PLoconMap ploconmap;

    if (_prePlace)
    {
	rflattenphfig(_prePlaceFig, YES, YES);
	for (phins* pins = _prePlaceFig->PHINS; pins; pins = pins->NEXT)
	{
	    fixedmap[pins->INSNAME] = 1;
	}

	_dx = _prePlaceFig->XAB1;
	_dy = _prePlaceFig->YAB1;

	for (phins* pins = _prePlaceFig->PHINS; pins; pins = pins->NEXT)
	{
	    // on elimine les tie et rowend
	    // qui peuvent etre dans la figure physique sans etre
	    // dans la figure logique ....
	    if (IsTie(pins))
	    {
		continue;
	    }
	    // on verifie que la loins existe en memoire
	    loins* ins = getloins(_fig, pins->INSNAME);
	    // verification de la coherence figure logique / figure physique
	    if (ins->FIGNAME != pins->FIGNAME)
	    {
		cerr << " o ERROR: The logical figure and the physical figure are not coherent:" << endl
		     << "   Logical Instance: " << ins->INSNAME << " -> logical model: "<< ins->FIGNAME << endl
		     << "   Physical Instance: " << pins->INSNAME << " -> physical model: "<< pins->FIGNAME << endl;
		exit(1);
	    }
	    PFixedIns* fixedins = InsertFixedIns(ins, pins, _dx, _dy);
	    pinsmap[ins->INSNAME] = fixedins;
	}

        if (_prePlaceFig->PHCON && 
                (_placeCons || _ringPlaceCons || _iocFile))
        {
            cerr << " o ERROR: impossible to have simultaneously preplaced connectors" << endl
                 << "          and automatically placed connectors" << endl;
            exit(1);
                
        }

        for (phcon* pcon = _prePlaceFig->PHCON; pcon; pcon = pcon->NEXT)
        {
            locon* lCon = getlocon(_fig, pcon->NAME);
            //generates an error if no locon
            PCon* pCon = InsertCon(lCon, pcon, _dx, _dy);
            pconmap[pcon->NAME] = pCon;
        }
    }
    
    
    for (loins* ins = fig->LOINS; ins; ins = ins->NEXT)
    {
	PFixedMap::iterator fit = fixedmap.find(ins->INSNAME);
	if (fit == fixedmap.end())
	{
	    // on evite de compter deux fois le meme net pour
	    // chaque instance.....
	    phfig* phmodel = getphfig(ins->FIGNAME, '0');
	    int height = (int)((double)(phmodel->YAB2 - phmodel->YAB1) / PITCH + 0.5);
	    if (height != ROWHEIGHT)
	    {
		cerr << " o ERROR : All cells must have the same height: i cannot place the instance: "
		     << ins->INSNAME << endl;
		exit(1);
	    }
	    PToPlaceIns* toplaceins = InsertToPlaceIns(ins);
	    pinsmap[ins->INSNAME] = toplaceins;
	}
    }
    _nInsToPlace = _toPlaceInss.size();

    // Traitement des connecteurs ....
    if (_placeCons || (_ringPlaceCons && !_iocFile))
    {
	for (locon* con = fig->LOCON; con; con = con->NEXT)
	{
	    losig* sig = con->SIG;
	    if (!IsSpecialNet(sig))
	    {
		PCon* pcon = InsertCon(con);
		pconmap[con->NAME] = pcon;
	    }
	}
    }
    
    for (locon* con = fig->LOCON; con; con = con->NEXT)
	ploconmap[con->NAME] = con;
    
    // traitement de l'emplacement des connecteurs
    // creation des PCon si fichier ioc
    if (_iocFile)
    {
	ParseIocFile(ploconmap);
	for (PCons::iterator pcon = _cons.begin() ; pcon != _cons.end(); pcon++)
	  pconmap[(*pcon)->GetLocon()->NAME] = *pcon;
    }

    for (losig* sig = fig->LOSIG; sig; sig = sig->NEXT)
    {
	if (!IsSpecialNet(sig)) 
	{
	    set<loins*> siginsset;
	    int totreatinscpt = 0;
	    int elemcpt = 0;
	    for (chain_list* it = (chain_list *)(getptype(sig->USER, (long)LOFIGCHAIN)->DATA);
		    it;
		    it = it->NEXT)
	    {
		locon_list* con = (locon_list*)(it->DATA);
		if (con->TYPE == EXTERNAL) {
		    if ( (_prePlaceFig && _prePlaceFig->PHCON)
                            || _placeCons || _ringPlaceCons || _iocFile) {
                        PConMap::iterator cit = pconmap.find(con->NAME);
                        if (cit != pconmap.end()) {
                            ++totreatinscpt;
                        }
		    }
		}
		else
		{
		    loins* ins = (loins *)con->ROOT;
		    set<loins*>::iterator liit = siginsset.find(ins);
		    if (liit == siginsset.end())
		    {
			++totreatinscpt;
			siginsset.insert(ins);
		    }
		}
		++elemcpt;
	    }
	    if (elemcpt < 2)
	    {
		cerr << " o OCP Warning: strange net detected ...: " << endl;
		cerr << " o Net:" << endl;
		cerr << " o ";
		PrintLosig(cerr, sig);
		if (elemcpt == 0)
		    cerr << " o is not connected ......" << endl;
		if (elemcpt == 1)
		{
		    cerr << " o is connected to only one element: ";
		    chain_list* it = (chain_list *)(getptype(sig->USER, (long)LOFIGCHAIN)->DATA);
		    locon_list* con = (locon_list*)(it->DATA);
		    if (con->TYPE == EXTERNAL)
		    {
			cerr << "connector " << con->NAME;
		    }
		    else
		    {
			loins* ins = (loins *)con->ROOT;
			cerr << "instance " << ins->INSNAME;
		    }
		    cerr << endl;
		}
		    
	    }

	    PONet* net = InsertNet(sig);
	    pnetmap[sig->INDEX] = net;
	}
	else {
	    cerr << " o Special Net detected : " << getsigname(sig) << endl;
	}
    }


    unsigned vectcpt = 0;
    for (loins* ins = fig->LOINS ; ins ; ins = ins->NEXT)
    {
	set<losig*> inssigset;
	PInsMap::iterator iit = pinsmap.find(ins->INSNAME);
	if (iit == pinsmap.end())
	{
	    cerr << "ins " << ins->INSNAME << " is not present in data...." << endl;
	    exit(1);
	}
	PElem::PNets& netvector = iit->second->GetNets();
	vectcpt = 0;
	for (locon* con = ins->LOCON ; con ; con = con->NEXT)
	{
	    losig* sig = con->SIG;
	    set<losig*>::iterator lsit = inssigset.find(sig);
	    if (lsit == inssigset.end())
	    {
		if (!IsSpecialNet(sig))
		{
		    PNetMap::iterator nit = pnetmap.find(sig->INDEX);
		    if (nit == pnetmap.end())
		    {
			cerr << "net " << sig->INDEX << " is not present in data...." << endl;
			exit(1);
		    }
		    netvector.push_back(nit->second);
		    inssigset.insert(sig);
		}
	    }
	}
    }

    if ( (_prePlaceFig && _prePlaceFig->PHCON)
            || _placeCons || _ringPlaceCons || _iocFile)
    {
	for (locon* con = fig->LOCON; con; con = con->NEXT)
	{
	    losig* sig = con->SIG;
	    if (!IsSpecialNet(sig))
	    {
		PConMap::iterator cit = pconmap.find(con->NAME);
		if (cit != pconmap.end())
		{
		  PElem::PNets& netvector = cit->second->GetNets();
		  PNetMap::iterator nit = pnetmap.find(sig->INDEX);
		  if (nit == pnetmap.end())
		  {
		      cerr << "net " << sig->INDEX
			  << " is not present in data...." << endl;
		      exit(1);
		  }
		  netvector.push_back(nit->second);
		}
	    }
	}
    }

    for (losig* sig = fig->LOSIG ; sig ; sig=sig->NEXT)
    {
	set<loins*> siginsset;
	if (!IsSpecialNet(sig)) 
	{
	    PNetMap::iterator nit = pnetmap.find(sig->INDEX);
	    if (nit == pnetmap.end())
	    {
		cerr << "net " << sig->INDEX << " is not present in data...." << endl;
		exit(1);
	    }
	    PNet::PElems& elemsvector = nit->second->GetElems();
	    for (chain_list* it = (chain_list *)(getptype(sig->USER, (long)LOFIGCHAIN)->DATA);
		    it;
		    it = it->NEXT)
	    {
		locon_list* con = (locon_list*)(it->DATA);
		if (con->TYPE == INTERNAL)
		{
		    loins* ins = (loins *)con->ROOT;
		    set<loins*>::iterator liit = siginsset.find(ins);
		    if (liit == siginsset.end())
		    {
			PInsMap::iterator iit = pinsmap.find(ins->INSNAME);
			if (iit == pinsmap.end())
			{
			    cerr << "ins " << ins->INSNAME << " is not present in data...." << endl;
			    exit(1);
			}
			elemsvector.push_back(iit->second);
			siginsset.insert(ins);
		    }
		}
		else if ( (_prePlaceFig && _prePlaceFig->PHCON)
                        || _placeCons || _ringPlaceCons || _iocFile)
		{
                    
		    PConMap::iterator cit = pconmap.find(con->NAME);
		    if (cit != pconmap.end())
		    {
			elemsvector.push_back(cit->second);
		    }
		}
		    
	    }
	}

    }

    if (_verbose)
    {
	cout << " o Number total of instances is ....  " << _fixedInss.size()+ _toPlaceInss.size() << endl;
	cout << " o Number of instances to place is ....  " << _toPlaceInss.size() << endl;
	cout << " o Number of instances already placed is ....  " << _fixedInss.size() << endl;
	cout << " o Number of nets is .... " << _nets.size() << endl;
    }
    
    // getting NetList Caracteristics

    _sumToPlaceInssWidth = 0.0;
    _biggestToPlaceInsWidth = 0.0;
    double inswidth = 0.0;
    for (PToPlaceInss::const_iterator ins_it = _toPlaceInss.begin();
	    ins_it != _toPlaceInss.end(); ins_it++)
    {
        if (_biggestToPlaceInsWidth < (inswidth = (*ins_it)->GetWidth()))
	    _biggestToPlaceInsWidth = inswidth;
        _sumToPlaceInssWidth += inswidth;
    }

    if (_verbose)
    {
        cout << " o Sum of instances to place widths is ... " << _sumToPlaceInssWidth << endl; 
    }
    
    _binsMaxWidth = (double)(int)(2.0 * _biggestToPlaceInsWidth * (1.0 + _margin) + 0.5);
    _binsMinWidth = (double)(int)(_binsMaxWidth / 2);

    if (_prePlace)
    {
	InitPlaceWithPrePlace();
	if (_nInsToPlace > 0)
	{
	    if (_rows.begin() == _rows.end())
	    {
		cout << " o ERROR: No SPACE to place instances ....." << endl;
		exit(1);
	    }
	}
    }
    else
	if (_nInsToPlace > 0)
	    InitPlace(NbRows);

    // set positions of connectors in case of no ioc file
    if (_placeCons && !_ringPlaceCons) GenerateConsPlacement();
    
    if (_ringPlaceCons && !_iocFile) GenerateRingConsPlacement();
    
    // set positions of connectors when ioc file is given
    if (_iocFile) SetPosIocFile(pconmap);
    
    if (_nInsToPlace==0)
	return;

    // sorting instances by their Width (biggest -> smallest)
    vector<PToPlaceIns*> ClassedInss;
    ClassedInss.reserve(_nInsToPlace);
    for (PToPlaceInss::iterator ifirst = _toPlaceInss.begin(); ifirst != _toPlaceInss.end(); ifirst++)
    {
        PToPlaceIns*   InsPt;
        ClassedInss.push_back(InsPt);
        ClassedInss.back() = *ifirst;
    }
    sort(ClassedInss.begin(), ClassedInss.end(), CompPInsByWidth());
    
    // Insertion of instances on rows
    vector<PToPlaceIns*>::iterator iptfirst = ClassedInss.begin();
    PRows::iterator rfirst = _rows.begin();
    PRow::PSubRows::iterator subrfirst = (*rfirst)->GetSubRows().begin();
    
    PIns* InsInserting = *iptfirst;
    while(1) 
    {
        int NbOfBins;
        int BinNumber;
            
        if (iptfirst == ClassedInss.end())
        {
            if (_verbose) cout << " o Initial Placement Computing ... done. " << endl;
            break;
            // end of insertion
        }

	if (subrfirst == (*rfirst)->GetSubRows().end()
		&& (++rfirst != _rows.end()))
	{
	    subrfirst = (*rfirst)->GetSubRows().begin();
	}
	
        if (rfirst == _rows.end()) 
        {
            if (InsInserting != *iptfirst)
            {
                InsInserting = *iptfirst;
                rfirst = _rows.begin();
		subrfirst = (*rfirst)->GetSubRows().begin();
            }
            else
            {
                // insertion of instances with respect of Bins margin
                // did not succed, inserting what's left.
                rfirst = _rows.begin();
		subrfirst = (*rfirst)->GetSubRows().begin();
                NbOfBins = (*subrfirst)->GetNBins();
                BinNumber = 0;
                while (iptfirst != ClassedInss.end()) 
		{
                    if ((*iptfirst)->GetWidth() <=
                           (*subrfirst)->GetWidth() - (*subrfirst)->GetSize())
                    {
                        (*subrfirst)->ForceIns(**(iptfirst++), BinNumber++);
                    }
                    else
                        BinNumber++;
                    if (BinNumber >= NbOfBins)
                    {
                        subrfirst++;
                        BinNumber = 0;
			if (subrfirst != (*rfirst)->GetSubRows().end())
			    NbOfBins = (*subrfirst)->GetNBins();
                    }

		    if (subrfirst == (*rfirst)->GetSubRows().end()
			    && (++rfirst != _rows.end()))
		    {
			subrfirst = (*rfirst)->GetSubRows().begin();
			NbOfBins = (*subrfirst)->GetNBins();
		    }
		    
                    if (rfirst == _rows.end())
                    {
                        if (InsInserting == *iptfirst)
                        {
                            // impossible to succed
                            cerr << " o The is no enough free space, I need a bigger abutment box ......" << endl;
                            cerr << " o There are still "
                                << ClassedInss.end() - iptfirst 
                                << " Instances to place ..." << endl;
                            exit(1);
                        }
                        rfirst = _rows.begin();
			subrfirst = (*rfirst)->GetSubRows().begin();
			NbOfBins = (*subrfirst)->GetNBins();
			BinNumber = 0;
                        InsInserting = *iptfirst;
                    }
                }
            }
        }
        NbOfBins = (*subrfirst)->GetNBins();
        BinNumber = 0;
        while ((iptfirst != ClassedInss.end())
                && (BinNumber < NbOfBins)
                && ((*iptfirst)->GetWidth() <= (*subrfirst)->GetCapa() - (*subrfirst)->GetSize()))
        {
            if ((*subrfirst)->InsertIns(**iptfirst, BinNumber++))
	    {
                iptfirst++;
	    }
        }
	subrfirst++;
    }

}

// ======================================================================
// void SetPosIocFile(PConMap& pconmap)
// ======================================================================
void
PPlacement::SetPosIocFile(PConMap& pconmap)
{
  int topcount 	  = 0;
  int bottomcount = 0;
  int rightcount  = 0;
  int leftcount   = 0;
  int topspace	  = 0;
  int bottomspace = 0;
  int rightspace  = 0;
  int leftspace   = 0;
  int nbspacetop    = 0;
  int nbspacebottom = 0;
  int nbspaceright  = 0;
  int nbspaceleft   = 0;

  con_list* pttop    = NULL;
  con_list* ptbottom = NULL;
  con_list* ptright  = NULL;
  con_list* ptleft   = NULL;

  PPos position;
  
  int Width = (int)(BBox.GetMaxX() - BBox.GetMinX());
  int Height = (int)(BBox.GetMaxY() - BBox.GetMinY());
  if (_verbose) cout << " o Width of the abutment box : " << Width << endl
    << " o Height of the abutment box : " << Height << endl;

  con_list* tmpnext;

  for(con_list* tmpcon = _PtList ; tmpcon; tmpcon = tmpnext)
  {
    tmpnext = tmpcon->NEXT;
    switch (tmpcon->ORIENT)
    {
      case 'T' : tmpcon->NEXT = pttop;
		 pttop = tmpcon;
		 if (strcmp(tmpcon->NAME,"SPACE")==0)
		 {
		   topspace += tmpcon->VALUE;
		   nbspacetop++;
		 } else topcount++;
		 break;
		 
      case 'B' : tmpcon->NEXT = ptbottom;
		 ptbottom = tmpcon;
		 if (strcmp(tmpcon->NAME,"SPACE")==0)
		 {
		   bottomspace += tmpcon->VALUE;
		   nbspacebottom++;
		 } else bottomcount++;
		 break;
		 
      case 'R' : tmpcon->NEXT = ptright;
		 ptright = tmpcon;
		 if (strcmp(tmpcon->NAME,"SPACE")==0)
		 {
		   rightspace += tmpcon->VALUE;
		   nbspaceright++;
		 } else rightcount++;
		 break;
		 
      case 'L' : tmpcon->NEXT = ptleft;
		 ptleft = tmpcon;
		 if (strcmp(tmpcon->NAME,"SPACE")==0)
		 {
		   leftspace += tmpcon->VALUE;
		   nbspaceleft++;
		 } else leftcount++;
		 break;	 
    }
  }

  if (topcount != 0)
  {
    double pos = 0.0;
    if (topspace >= Width-topcount)
    {
      cerr << " o ERROR : in ioc file : space too important" << endl;
      exit(1);
    }
    double conspace = ((double)(Width-topspace)/(double)(topcount-nbspacetop));
    pos += conspace / 2.0;

    if (_verbose)
	cout << " o conspace : " << conspace
	    << " 1st connector : " << pos << endl;

    for(con_list* tmpcon = pttop ; tmpcon; tmpcon = tmpcon->NEXT)
    {
      if (strcmp(tmpcon->NAME,"SPACE") == 0)
      {
	pos += (double)tmpcon->VALUE;
	pos -= conspace;
      }
      else
      {
	if (_verbose) cout << " o adding connector  : " << tmpcon->NAME
	  << " x : " << (int)pos << " y : " << Height << endl;
	position.SetX((double)(int)pos);
	position.SetY(Height);
	// test a rajouter pour verifier si locon present ou pas!!!
	pconmap[tmpcon->NAME]->SetPos(position);
	pos += conspace;
      }
    }
  }

  if (bottomcount != 0)
  {
    double pos = 0.0;
    if (bottomspace >= Width-topcount)
    {
      cerr << " o ERROR : in ioc file : space too important" << endl;
      exit(1);
    }
    double conspace = ((double)(Width-bottomspace)/
					(double)(bottomcount-nbspacebottom));
    pos += conspace / 2.0;

    if (_verbose) cout << " o conspace : " << conspace << " 1st connector : " << pos << endl;
    
    for(con_list* tmpcon = ptbottom ; tmpcon; tmpcon = tmpcon->NEXT)
    {
      if (strcmp(tmpcon->NAME,"SPACE") == 0)
      {
	pos += (double)tmpcon->VALUE;
	pos -= conspace;
      }
      else
      {
	if (_verbose) cout << " o adding connector : " << tmpcon->NAME
	  << " x : " << (int)pos << " y : " << 0 << endl;
	position.SetX((double)(int)pos);
	position.SetY(0.0);
	// test a rajouter pour verifier si locon present ou pas!!!
	pconmap[tmpcon->NAME]->SetPos(position);
	pos += conspace;
      }
    }
  }

  if (leftcount != 0)
  {
    double pos = 0.0, ppos = pos;
    if (leftspace >= Width-topcount)
    {
      cerr << " o ERROR : in ioc file : space too important" << endl;
      exit(1);
    }
    double conspace = ((double)(Height-leftspace)/
					(double)(leftcount-nbspaceleft));
    if (_ringPlaceCons) ppos = pos;
    pos += conspace / 2.0;

    for(con_list* tmpcon = ptleft ; tmpcon; tmpcon = tmpcon->NEXT)
    {
      if (strcmp(tmpcon->NAME,"SPACE") == 0)
      {
	pos += (double)tmpcon->VALUE;
	pos -= conspace;
      }
      else
      {
        if (_ringPlaceCons)
        {
	int delta = (int)pos % ROWHEIGHT;
          if      (delta == 0) pos -=  2.0;
          else if (delta == 1) pos +=  1.0;
          else if (delta == 9) pos -=  1.0;
          // Just in case there are so many connectors
          if (pos < ppos) pos = ppos + 1;
        }

	if (_verbose) cout << " o adding connector : " << tmpcon->NAME
	  << " x : " << 0 << " y : " << (int)pos << endl;
	position.SetX(0.0);
	position.SetY((double)(int)pos);
	// test a rajouter pour verifier si locon present ou pas!!!
	pconmap[tmpcon->NAME]->SetPos(position);
        if (_ringPlaceCons) ppos = pos;
	pos += conspace;
      }
    }
  }

  if (rightcount != 0)
  {
    double pos = 0.0, ppos = pos;
    if (rightspace >= Width-topcount)
    {
      cerr << " o ERROR : in ioc file : space too important" << endl;
      exit(1);
    }
    double conspace = ((double)(Height-rightspace)/
					(double)(rightcount-nbspaceright));
    if (_ringPlaceCons) ppos = pos;
    pos += conspace / 2.0;

    for(con_list* tmpcon = ptright ; tmpcon; tmpcon = tmpcon->NEXT)
    {
      if (strcmp(tmpcon->NAME,"SPACE") == 0)
      {
	pos += (double)tmpcon->VALUE;
	pos -= conspace;
      }
      else
      {
        if (_ringPlaceCons)
        {
	int delta = (int)pos % ROWHEIGHT;
         if      (delta == 0) pos -=  2.0;
         else if (delta == 1) pos +=  1.0;
         else if (delta == 9) pos -=  1.0;
	 // Just in case there are so many connectors
	 if (pos < ppos) pos = ppos + 1;
        }

	if (_verbose) cout << " o adding connector: " << tmpcon->NAME
	  << " x : " << Width << " y : " << (int)pos << endl;
	position.SetX(Width);
	position.SetY((double)(int)pos);
	// test a rajouter pour verifier si locon present ou pas!!!
	pconmap[tmpcon->NAME]->SetPos(position);
        if (_ringPlaceCons) ppos = pos;
	pos += conspace;
      }
    }
  }
}
// ======================================================================
// void ParseIocFile(PLoconMap& ploconmap)
// ======================================================================

void
PPlacement::ParseIocFile(PLoconMap& ploconmap)
{
    PPos position;

    _PtList = iocparse(_iocFileName);

    char orientation;

    for(con_list* tmpcon = _PtList ; tmpcon ; tmpcon = tmpcon->NEXT)
    {
	if (strcmp(tmpcon->NAME,"SPACE") != 0)
	{
	    if (ploconmap.find(tmpcon->NAME) == ploconmap.end())
	    {
		cerr << " o Error in ioc file ...." << endl
		    << "   The connector " << tmpcon->NAME
		    << " doesn't exist in your figure ...."
		    << endl;
		exit(1);
	    }
	    switch (tmpcon->ORIENT)
	    {
		case 'T':
		    orientation = NORTH;
		    break;
		case 'B':
		    orientation = SOUTH;
		    break;
		case 'L':
		    orientation = WEST;
		    break;
		case 'R':
		    orientation = EAST;
		    break;
		default:
		    cerr << "Unknown Orientation for connector: "
			<< tmpcon->NAME << endl;
		    exit(1);
	    }
	    position.SetX(0);
	    position.SetY(0);
	    InsertCon(ploconmap[tmpcon->NAME], position, orientation);
	}
    }
}

// ======================================================================
// void Place(lofig* fig, int NbRows);
// ======================================================================
void
PPlacement::Place(lofig* fig, int NbRows)
{
    Init(fig, NbRows);
    if (_boolPlot)
    {
	PlotOnlyInstances("instances-init");
	PlotAll("init");
    }
    if (_nInsToPlace > 0)
    {
	PlaceGlobal();
	if (_boolPlot)
	{
	  PlotAll("afterglobal");
	  PlotOnlyBins("binsafterglobal");
	}
	PlaceFinal();
    }
}

PFixedIns*
PPlacement::InsertFixedIns(const loins* ins, const phins* pins, const int dx, const int dy)
{
    PFixedIns* fixedins = new PFixedIns(ins, pins, dx, dy);
    _fixedInss.push_back(fixedins);
    return fixedins;
}

PToPlaceIns*
PPlacement::InsertToPlaceIns(const loins* ins)
{
    PToPlaceIns* toplaceins = new PToPlaceIns(ins);
    _toPlaceInss.push_back(toplaceins);
    return toplaceins;
}

PCon*
PPlacement::InsertCon(const locon* con, phcon* phycon, int dx, int dy)
{
    PCon* pcon = new PCon(con, phycon);
    _cons.push_back(pcon);
    return pcon;
}

PCon*
PPlacement::InsertCon(const locon* con, PPos position, const char orientation)
{
    PCon* pcon = new PCon(con, position, orientation);
    _cons.push_back(pcon);
    return pcon;
}

PONet*
PPlacement::InsertNet(const losig* sig)
{
    PONet* net = new PONet(sig);
    _nets.push_back(net);
    return net;
}

ostream&
PPlacement::Print(ostream& os) const
{
    PToPlaceInss::const_iterator ifirst = _toPlaceInss.begin();
    PToPlaceInss::const_iterator ilast  = _toPlaceInss.end();
    while (ifirst != ilast) {
	const PToPlaceIns& ins = **ifirst++;
	os << " + ins: " << ins << endl;
	PIns::PNets::const_iterator first = ins.GetConstNets().begin();
	PIns::PNets::const_iterator last  = ins.GetConstNets().end();
	while (first != last)
	    os << "   - net: " << *first++ << endl;
    }

    PONets::const_iterator nfirst = _nets.begin();
    PONets::const_iterator nlast  = _nets.end();
    while (nfirst != nlast) {
	const PNet& net = **nfirst++;
	os << " + net: " << net << endl;
	PNet::PElems::const_iterator first = net.GetConstElems().begin();
	PNet::PElems::const_iterator last  = net.GetConstElems().end();
	while (first != last)
	    os << "   - ins: " << *first++ << endl;
    }

    return os;
}

// ======================================================================
// void PlotInstances(ofstream& out) const
// ======================================================================
void
PPlacement::PlotInstances(ofstream& out) const
{
  
  out << "#rows" << endl;
  for (PRows::const_iterator rit=_rows.begin(); rit!=_rows.end(); rit++)
  {
      (*rit)->Plot(out);
  }
  out << "EOF" << endl;
  out << "#to place instances" << endl;
  for (PToPlaceInss::const_iterator insit = _toPlaceInss.begin();
	  insit != _toPlaceInss.end(); insit++)
  {
      (*insit)->Plot(out);
  }
  out << "EOF" << endl;
  out << "#preplaced instances" << endl;
  for (PFixedInss::const_iterator insit = _fixedInss.begin();
	  insit != _fixedInss.end(); insit++)
  {
      (*insit)->Plot(out);
  }
  out << "EOF" << endl;
}
// ======================================================================
// void PlotAll(char *output) const
// ======================================================================
void
PPlacement::PlotAll(char* output) const
{
    string name(output);
    name += ".gpl";
    ofstream out(name.c_str());

    out << "set noxtics\nset noytics\n"
	<< "set noborder\nset nokey\nset title '" << _fig->NAME << "'\n"
	<< "#set terminal postscript eps color solid\n#set output '"
	<< output << ".ps'\n"
	<< "set xrange[" << GetMinX() << ":" << GetMaxX() << "]\n"
	<< "set yrange[" << GetMinY() << ":" << GetMaxY() << "]\n"
	<< "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3, '-' w l 4\n"
	<< GetMinX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMinY() << endl << endl;

    PlotInstances(out);
  
    out << "#nets" << endl;
    for (PONets::const_iterator nit=_nets.begin(); nit!=_nets.end(); nit++)
    {
	(*nit)->Plot(out);
    }
    out << "EOF" << endl << "pause -1 'press any key'" << endl;
}

// ======================================================================
// void PlotFinal(char *output) const
// ======================================================================
void
PPlacement::PlotFinal(char* output) const
{
    string name(output);
    name += ".gpl";
    ofstream out(name.c_str());

    out << "set noxtics\nset noytics\n"
	<< "set noborder\nset nokey\nset title '" << _fig->NAME << "'\n"
	<< "#set terminal postscript eps color solid\n#set output '"
	<< output << ".ps'\n"
	<< "set xrange[" << GetMinX() << ":" << GetMaxX() << "]\n"
	<< "set yrange[" << GetMinY() << ":" << GetMaxY() << "]\n"
	<< "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3, '-' w l 4\n"
	<< GetMinX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMinY() << endl << endl;

    out << "#preplaced instances" << endl;
    for (PFixedInss::const_iterator insit = _fixedInss.begin();
	    insit != _fixedInss.end(); insit++)
    {
	(*insit)->Plot(out);
    }
    out << "EOF" << endl;

    out << "#subrows" << endl;
    for (PDetSubRows::const_iterator srit = _detSubRows.begin() ;
	  srit != _detSubRows.end() ;
	  srit++)
    {
	(*srit)->Plot(out);
    }
    out << "EOF" << endl;

    out << "#instances" << endl;

    for (PDetSubRows::const_iterator srit = _detSubRows.begin() ;
	    srit != _detSubRows.end() ;
	    srit++)
    {
	for (PDetSubRow::PDetInsVector::const_iterator iit = (*srit)->GetConstInssVector().begin() ;
		iit != (*srit)->GetConstInssVector().end() ;
		iit++)
	{
	    (*iit)->Plot(out);
	}
    }
    out << "EOF" << endl;
    out << "#nets" << endl;

    for (PONets::const_iterator nit=_nets.begin(); nit!=_nets.end(); nit++)
    {
	(*nit)->Plot(out);
    }
    out << "EOF" << endl << "pause -1 'press any key'" << endl;
}
// ======================================================================
// void PlotOnlyInstances(char* output) const
// ======================================================================
void
PPlacement::PlotOnlyInstances(char* output) const
{
    string name(output);
    name += ".gpl";
    ofstream out(name.c_str());

    out << "set noxtics\nset noytics\n"
	<< "set noborder\nset nokey\nset title '" << _fig->NAME << "'\n"
	<< "#set terminal postscript eps color solid\n#set output '"
	<< output << ".ps'\n";
    out << "set xrange[" << GetMinX() << ":" << GetMaxX() << "]\n"
	<< "set yrange[" << GetMinY() << ":" << GetMaxY() << "]\n"
	<< "plot [:][:][:] '-' w l, '-' w l 2, '-' w l 3\n"
	<< GetMinX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMaxY() << endl
	<< GetMaxX() << " " << GetMinY() << endl
	<< GetMinX() << " " << GetMinY() << endl << endl;
    PlotInstances(out);
    out << "pause -1 'press any key'" << endl;
}
// ======================================================================
// void PlotOnlyBins(char* output) const
// ======================================================================
void
PPlacement::PlotOnlyBins(char* output) const
{  
    string name (output);
    name += ".gpl";
    ofstream out(name.c_str());

    out << "set noxtics\nset noytics\n"
	<< "set noborder\nset nokey\nset title '" << _fig->NAME << "'\n"
	<< "#set terminal postscript eps color solid\n#set output '"
	<< output << ".ps'\n";

    if (_totalMoves != 0)
    {
	for (PRows::const_iterator rit=_rows.begin(); rit!=_rows.end(); rit++)
	{
	    (*rit)->PlotLabel(out, _totalMoves);
	}
    }

    out << "plot [:][:] '-' w l 1, '-' w l 2" << endl << "#bins" << endl;
    for (PRows::const_iterator rit=_rows.begin(); rit!=_rows.end(); rit++)
    {
	(*rit)->Plot(out);
    }

    out << "EOF" << endl;
    out << "#preplaced instances" << endl;
    for (PFixedInss::const_iterator iit = _fixedInss.begin();
	    iit != _fixedInss.end(); iit++)
    {
	(*iit)->Plot(out);
    }
    out << "EOF" << endl;
    out << "pause -1 'press any key'" << endl;
}
// ======================================================================
// void PlotStat()
// ======================================================================
void
PPlacement::PlotStat()
{
    ofstream out("stat.gpl");

  out << "set title 'all stats'" << endl
    << "#set terminal postscript color solid" << endl
    << "#set output 'stat.ps'" << endl
    << "plot 'alldata.dat' using 1:2 title 'temperature' with lines,"
    << "'alldata.dat' using 1:3 title 'cost' with lines,"
    << "'alldata.dat' using 1:4 title 'rowcost' with lines,"
    << "'alldata.dat' using 1:5 title 'bincost' with lines,"
    << "'alldata.dat' using 1:6 title 'netcost' with lines,"
    << "'alldata.dat' using 1:7 title 'success ratio' with lines,"
    << "'alldata.dat' using 1:8 title 'dist' with lines,"
    << "'alldata.dat' using 1:9 title 'delta' with lines" << endl
    << "pause -1 'press any key'" << endl;
}

// ======================================================================
// void GenerateConsPlacement()
// ======================================================================
void
PPlacement::GenerateConsPlacement()
{
    //creation d'un placement aleatoire des connecteurs.
    unsigned faces[4];
    double aspectRatio = GetWidth() / GetHeight();
    double repartition = aspectRatio + 1.0;
    unsigned nbConsNorthSouth = (unsigned)( aspectRatio * _cons.size() / repartition);
    unsigned nbConsEastWest = _cons.size() - nbConsNorthSouth;
    
    //faces north est sud ouest
    faces[0] = nbConsNorthSouth / 2;
    faces[2] = nbConsNorthSouth - faces[0];
    faces[1] = nbConsEastWest / 2;
    faces[3] = nbConsEastWest - faces[1];
    
    PCons::iterator cit = _cons.begin();

    unsigned minx = (unsigned)BBox.GetMinX();
    unsigned miny = (unsigned)BBox.GetMinY(); 
    unsigned maxx = (unsigned)BBox.GetMaxX();
    unsigned maxy = (unsigned)BBox.GetMaxY();

    unsigned pos = 0;
    unsigned interval = 0;
    int spaceLeft = 0;
    
    //placement au nord.
    if (faces[0])
    {
	interval = (maxx - minx) / faces[0];
	spaceLeft = (maxx - minx) % faces[0];
	pos = minx + interval / 2;
	while(faces[0]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(NORTH);
	    (*cit++)->SetPos(PPos(pos , maxy));
	    pos += interval;
	}
    }
    
    //placement a l'est.
    if (faces[1])
    {
	interval = (maxy - miny) / faces[1];
	spaceLeft = (maxy - miny) % faces[1];
	pos = miny + interval / 2;
	while(faces[1]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(EAST);
	    (*cit++)->SetPos(PPos(maxx , pos));
	    pos += interval;
	}
    }
    
    //placement au sud.
    if (faces[2])
    {
	interval = (maxx - minx) / faces[2];
	spaceLeft = (maxx - minx) % faces[2];
	pos = minx + interval / 2;
	while(faces[2]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(SOUTH);
	    (*cit++)->SetPos(PPos(pos , miny));
	    pos += interval;
	}
    }
    
    //placement a l'ouest.
    if (faces[3])
    {
	interval = (maxy - miny) / faces[3];
	spaceLeft = (maxy - miny) % faces[3];
	pos = miny + interval / 2;
	while(faces[3]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(WEST);
	    (*cit++)->SetPos(PPos(minx , pos));
	    pos += interval;
	}
    }
}

// ======================================================================
// void GenerateRingConsPlacement()
// this method will be deleted in the future ( .... I HOPE ;) .. )
// His purpose is to give a ring suitable connectors placement
// ======================================================================
void
PPlacement::GenerateRingConsPlacement()
{
    //creation d'un placement aleatoire des connecteurs pour ring.
    unsigned faces[2];
    unsigned nbConsNorth = _cons.size() / 2;
    unsigned nbConsSouth = _cons.size() - nbConsNorth;
    
    //faces north est sud ouest
    faces[0] = nbConsNorth;
    faces[1] = nbConsSouth;
    
    PCons::iterator cit = _cons.begin();

    unsigned minx = (unsigned)BBox.GetMinX();
    unsigned miny = (unsigned)BBox.GetMinY(); 
    unsigned maxx = (unsigned)BBox.GetMaxX();
    unsigned maxy = (unsigned)BBox.GetMaxY();

    unsigned pos = 0;
    unsigned interval = 0;
    int spaceLeft = 0;
    
    //placement au nord.
    if (faces[0])
    {
	interval = (maxx - minx) / faces[0];
	spaceLeft = (maxx - minx) % faces[0];
	pos = minx + interval / 2;
	while(faces[0]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(NORTH);
	    (*cit++)->SetPos(PPos(pos , maxy));
	    pos += interval;
	}
    }
    
    //placement au sud.
    if (faces[1])
    {
	interval = (maxx - minx) / faces[1];
	spaceLeft = (maxx - minx) % faces[1];
	pos = minx + interval / 2;
	while(faces[1]--)
	{
	    if (spaceLeft-- > 0)
		pos += 1;
	    (*cit)->SetOrient(SOUTH);
	    (*cit++)->SetPos(PPos(pos , miny));
	    pos += interval;
	}
    }
}
    
// ======================================================================
// void InitPlace(int NbRows)
// private method
// ======================================================================
void
PPlacement::InitPlace(int NbRows)
{
    // no preplacement
    // we make a rectangle of rows with for each one subrow
    // of size = size of the row
    // composed of bins of constant size
    int NRows = NbRows;
    
    if (_verbose)
	cout << " o Computing Initial Placement ..." << endl;
    
    if (_verbose)
    {
	cout << " o User Margin : " << 100.0 * _margin << "%" << endl;
    }
    
    double rowwidth = SquareShape(_margin, _sumToPlaceInssWidth,
	    _binsMinWidth, NRows);
    
    if (_verbose)
    {
      cout << " o Number of Rows : " << NRows << endl;
    }
    
    // computing Effective Margin
    double realSurface = NRows * rowwidth;
    _realMargin = 1.0 - _sumToPlaceInssWidth / realSurface;
    if (_verbose)
    {
        cout << " o Real Margin : " << 100.0 * _realMargin << "%" << endl;
    }
        
        
    _rows.reserve(NRows);
    
    double Y = 0.0;
    unsigned rowidx = 0;
    bool RowOrientation = false;
    _rowZeroOrientation = RowOrientation;
    for (int i = 0; i < NRows; i++)
    {
	_rows.push_back(new PRow(1));
        double XMin = 0.0;
	_rows.back()->Init(Y, XMin, *this, RowOrientation);
	_rowsYMax[Y + ROWHEIGHT] = rowidx;
	_rowsYMinInv[Y] = rowidx;
	++rowidx;
	PSubRow& subrow = *(_rows.back()->_subRows[0]);
	subrow.Init(_rows.back(), Y, XMin, rowwidth, _realMargin, _binsMaxWidth, _binsMinWidth);
	_rows.back()->_subRowsXMax[rowwidth + XMin] = 0;
	_rows.back()->_subRowsXMaxInv[rowwidth + XMin] = 0;
        Y += ROWHEIGHT;
	RowOrientation = !RowOrientation;
    }

    // Computing Placement BBox
    double MaxX = 0.0, MaxY = 0.0, RowMaxX, RowMaxY;
    for (PRows::iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
        RowMaxX = (*rit)->GetMaxX();
        RowMaxY = (*rit)->GetMaxY();
        if (MaxX < RowMaxX)
            MaxX = RowMaxX;
        if (MaxY < RowMaxY)
            MaxY = RowMaxY;
    }
    BBox.SetMinX(0.0);
    BBox.SetMinY(0.0);
    BBox.SetMaxX(RowMaxX);
    BBox.SetMaxY(RowMaxY);
}

// ======================================================================
// void InitPlaceWithPrePlace()
// private method
// ======================================================================
void
PPlacement::InitPlaceWithPrePlace()
{
    // abutment box
    if (!_prePlaceFig)
    {
        cerr << " o ERROR: impossible to init preplacement with empty preplacement figure" << endl;
        exit(1);
    }

    int Width = _prePlaceFig->XAB2 - _prePlaceFig->XAB1;
    int Height = _prePlaceFig->YAB2 - _prePlaceFig->YAB1;

    // VERIFICATION OF THE PREPLACEMENT
    // abutment box
    if (Width % PITCH != 0)
    {
	cerr << " o ERROR : abutment box's width must be a multiple of 5 lambdas"
	    << endl;
	exit(1);
    }
    
    if (Height % (PITCH * ROWHEIGHT) != 0)
    {
	cerr << " o ERROR : abutment box's height must be a multiple of 50 lambdas"
	    << endl;
	exit(1);
    }
    
    Width =(int)(Width / PITCH);
    Height = (int)(Height / (PITCH * ROWHEIGHT));

    PrePlaceTab tabpreplace(Height, PrePlaceRow(Width, false));

    // find the orientation of the first row.
    phins* refpins = _prePlaceFig->PHINS;

    if (refpins)
    {
        int ycoord = (int)((refpins->YINS - _dy) / (ROWHEIGHT * PITCH));
        phfig* refpinsmodel = getphfig(refpins->FIGNAME, 'P');
        int refpinsheight = (int)((refpinsmodel->YAB2 - refpinsmodel->YAB1)
                / (ROWHEIGHT * PITCH));
        char transf = refpins->TRANSF;

        if (ycoord % 2 == 0)
        {
          if ((refpinsheight == 2) || (transf == NOSYM) || (transf == SYM_X))
            _rowZeroOrientation = true;
          else
            _rowZeroOrientation = false;
        } else {
          if ((refpinsheight == 2) || (transf == NOSYM) || (transf == SYM_X))
            _rowZeroOrientation = false;
          else
            _rowZeroOrientation = true;
        }

        // tests for each instance
        for (phins* pins = _prePlaceFig->PHINS; pins; pins = pins->NEXT)
        {
            phfig_list* phmodel = getphfig(pins->FIGNAME, '0');
            int pinswidth = phmodel->XAB2 - phmodel->XAB1;
            int pinsheight = phmodel->YAB2 - phmodel->YAB1;
            
            pinswidth = (int)(pinswidth / PITCH);			// largeur ramene au pitch
            pinsheight = (int)(pinsheight / PITCH);			// hauteur ramene au pitch
            int pinsheightrow = (int)(pinsheight / ROWHEIGHT);	// hauteur ramene a l'unite
                                                                    // (taille des lignes)
            int ypos = (int)((pins->YINS - _dy) / PITCH);		// position en y ramene au pitch
            int xpos = (int)((pins->XINS - _dx) / PITCH);		// position en x ramene au pitch
            int ycoord = (int)(ypos / ROWHEIGHT);			// position en y ramene a l'unite

            if ((pins->YINS - _dy) % 50 != 0)
            {
                cerr << " o ERROR : in preplacement file : y position of "
                    << pins->INSNAME << " is incorrect" << endl;
                exit (1);
            }

            if ((pins->XINS - _dx) % 5 != 0)
            {
                cerr << " o ERROR : in preplacement file : x position of "
                    << pins->INSNAME << " is incorrect" << endl;
                exit (1);
            }

            if (   (pins->TRANSF == ROT_P) || (pins->TRANSF == ROT_M)
                || (pins->TRANSF == SY_RP) || (pins->TRANSF == SY_RM) )
            {
                cerr << " o ERROR : " << pins->INSNAME << " : incorrect rotation" << endl;
                exit(1);
            }
            
            // check if orientation of instance matches
            bool insOrient;
            if ((pinsheightrow == 2) || (pins->TRANSF == NOSYM) || (pins->TRANSF == SYM_X))
              insOrient = true;
            else
              insOrient = false;

            if (   ((ycoord % 2 == 0) && (insOrient != _rowZeroOrientation))
                || ((ycoord % 2 != 0) && (insOrient == _rowZeroOrientation)) )
            {
              cerr << " o ERROR : " << pins->INSNAME << " badly oriented" << endl
                   << "   Incoherence with " << refpins->INSNAME << endl;
              exit(1);
            }
            
            for (int yit = ycoord; yit < ycoord + pinsheightrow; yit++)
            {
              for (int xit = xpos; xit < xpos + pinswidth; xit++)
              {
                if (   (xit > Width - 1) || (yit > Height - 1)
                    || (xit < 0    ) || (yit < 0     ) )
                {
                  cerr << " o ERROR : " << pins->INSNAME
                       << " out of the abutment box" << endl;
                  exit(1);
                }
                
                if (tabpreplace[yit][xit] == false)
                {
                    tabpreplace[yit][xit] = true;
                }
                else{
                  cerr << " o ERROR : " << pins->INSNAME << " badly placed .... There is already an instance at its position ...." << endl;
                  exit (1);
                }
              }
            }
        }
    }

    // create rows and subrows
    if (_nInsToPlace > 0)
    {
	int nbrows = 0;
	for (int yit = 0; yit < Height; yit++)
	{
	  if (CheckCreateRow(tabpreplace, yit, Width) > 0) ++nbrows;
	}


	_rows.reserve(nbrows);

	bool orientation = _rowZeroOrientation;
	int numrows = 0;
	for (int yit = 0; yit < Height; yit++)
	{
	  int numsubrows = CheckCreateRow(tabpreplace, yit, Width);
	  if (numsubrows > 0)
	  {
            PRow* row = new PRow(*this, (double)(yit * ROWHEIGHT), Width, numsubrows, orientation);
            _rows.push_back(row);
	    _rowsYMax[(yit + 1) * ROWHEIGHT] = numrows;
	    _rowsYMinInv[yit * ROWHEIGHT] = numrows;
	    ++numrows;
	    CreateSubRows(_rows.back(), tabpreplace, yit, numsubrows, Width);
	  }
	  orientation = !orientation;
	}
    }
    BBox.SetMinX(0.0);
    BBox.SetMinY(0.0);
    BBox.SetMaxX(Width);
    BBox.SetMaxY(Height * ROWHEIGHT);
}

// ======================================================================
// void CreateSubRows(PRow* row, PrePlaceTab& tabpreplace,int coordy, int nbsubrows, int Width)
// private method
// ======================================================================
void
PPlacement::CreateSubRows(PRow* row, PrePlaceTab& tabpreplace,
   					int coordy, int nbsubrows, int Width)
{
  int coordx = 0;
  int subrowscreated = 0;

  while (coordx < Width)
  {
    while (( coordx < Width  			  )
	&& ( tabpreplace[coordy][coordx] == true ))
      ++coordx;

    if (coordx >= Width)
	break;

    int xmin = coordx;
	
    while (( coordx < Width  			 )
	&& ( tabpreplace[coordy][coordx] == false ))
      ++coordx;

    int xmax = coordx;

    if ( (xmax - xmin) >= _binsMinWidth)
    {
	row->_subRows[subrowscreated]->Init(row, (double)(coordy * ROWHEIGHT), (double)xmin,(double)xmax,
		(double)_margin, _binsMaxWidth, _binsMinWidth);
	row->_subRowsXMax[(double)xmax] = subrowscreated;
	++subrowscreated;
    }
    for (PRow::PSubRowsXMax::iterator srymit = row->_subRowsXMax.begin();
	    srymit != row->_subRowsXMax.end();
	    srymit++)
    {
	row->_subRowsXMaxInv[srymit->first] = srymit->second;
    }
  }
  if (subrowscreated != nbsubrows)
  {
      cerr << " o INTERNAL ERROR : in subrow creation" << endl;
      exit (1);
  }
}

// ======================================================================
// void CheckCreateRow(PrePlaceTab& tabpreplace, int coordy, int Width)
// private method
// ======================================================================
int
PPlacement::CheckCreateRow(PrePlaceTab& tabpreplace, int coordy, int Width)
{
  int coordx = 0;
  int nbsubrows = 0;
  
  while (coordx < Width)
  {
    while (  ( coordx < Width  			)
	  && ( tabpreplace[coordy][coordx] == true	))
      ++coordx;

    if (coordx >= Width)
	break;
	
    int xmin = coordx;

    while (  ( coordx < Width  			)
	  && ( tabpreplace[coordy][coordx] == false	))
      ++coordx;

    int xmax = coordx;

    if ( (xmax - xmin) >= _binsMinWidth)
	++nbsubrows;
  }

  return (nbsubrows);
}

// ======================================================================
// PRow& GetRow(const PRow* row, const double dist)
// private method
// ======================================================================
PRow&
PPlacement::GetRow(const PRow* row, const double dist)
{
    // returns randomly a row at maximum distance from yorig
    double bornesup, borneinf;

    
    if ((bornesup = (row->GetMinY() + row->GetHeight() / 2.0) + (double)(int)(dist * GetHeight() + 0.5) ) > GetMaxY())
        bornesup = GetMaxY();

    if ((borneinf = (row->GetMinY() + row->GetHeight() / 2.0) - (double)(int)(dist * GetHeight() + 0.5) ) < GetMinY())
        borneinf = GetMinY();

    PRowsYMax::iterator rinf = _rowsYMax.upper_bound(borneinf);
    PRowsYMax::iterator rsup = _rowsYMinInv.upper_bound(bornesup);

    unsigned randidx = rinf->second + (unsigned)((double)(rsup->second - rinf->second + 1) * (rand() / (RAND_MAX+1.0)));
    return *_rows[randidx];
}

double PPlacement::GetBinsCapa() const
{
    double binsCapa = 0.0;
    for (PRows::const_iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
        binsCapa += (*rit)->GetBinsCapa();
    }
    return binsCapa;
}

double PPlacement::GetSubRowsCapa() const
{
    double subRowsCapa = 0.0;
    for (PRows::const_iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
        subRowsCapa += (*rit)->GetSubRowsCapa();
    }
    return subRowsCapa;
}

double PPlacement::GetBinsSize() const
{
    double binsSize = 0.0;
    for (PRows::const_iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
        binsSize += (*rit)->GetBinsSize();
    }
    return binsSize;
}
