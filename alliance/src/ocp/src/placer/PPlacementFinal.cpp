#include "PPlacement.h"
#include <math.h>
#include <unistd.h>
#include <algorithm>
#include <hash_map.h>
#include "PMove.h"
#include "PConstants.h"
#include "PDetPlacement.h"
extern "C"{
#include "mut.h"
#include "mph.h"
#include "mpu.h"
#include "mlo.h"
#include "mlu.h"
}

double
PPlacement::DetPlaceDebugNetCost()
{
    double NetCost = 0.0;

    vector<PONet*>::iterator nfirst = _nets.begin();
    vector<PONet*>::iterator nlast  = _nets.end();
    while (nfirst != nlast) {
	PONet& net = **nfirst++;
	PBBox bbox;
	vector<PElem*>::iterator ifirst = net.GetElems().begin();
	vector<PElem*>::iterator ilast  = net.GetElems().end();
	if (ifirst != ilast)
	{
	    while (ifirst != ilast)
		bbox.Merge((*ifirst++)->GetPos());
	    
	    double width = bbox.GetWidth();

	    NetCost += bbox.GetHeight() + width;
	}
    }
    return NetCost;
}

void
PPlacement::PlaceFinal()
{
    // Detailed Placement
    if (_verbose) cout << " o Final Optimization in process ..." << endl;
    FinalInitialize();
    if (_verbose)
    {
      if (FinalOptimize())
	  cout << " o Final Optimization succeeded ..." << endl;
      else
	  cout << " o No Optimization possible" << endl;
    }
    double NewCost = DetPlaceDebugNetCost();
    if (_verbose)
    {
      cout << " o Final Net Cost ..... " << NewCost << endl;
      cout << " o Final Net Cost Optimization ..... " 
	<< 100.0 * (_detInitNetCost - NewCost) / _detInitNetCost << "%" << endl;
      cout << " o Total Net Optimization .... "
	  << 100.0 * (_initNetCost - NewCost) / _initNetCost << "%" << endl;
    }
}

// ======================================================================
// double FinalInitialize()
// ======================================================================
void
PPlacement::FinalInitialize()
{
    // on compte le nombre de subrow
    // on prend uniquement celle qui ont des instances
    unsigned nbsubrows = 0;
    for (PRows::iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
	for (PRow::PSubRows::iterator srit = rit->GetSubRows().begin();
		srit != rit->GetSubRows().end();
		srit++)
	{
	    if (srit->GetNIns() > 0)
		++nbsubrows;
	}
    }
    _detSubRows.reserve(nbsubrows);

    for (PRows::iterator rit = _rows.begin(); rit != _rows.end(); rit++)
    {
	for (PRow::PSubRows::iterator srit = rit->GetSubRows().begin();
		srit != rit->GetSubRows().end();
		srit++)
	{
	    if (srit->GetNIns() > 0)
		_detSubRows.push_back(PDetSubRow(*srit));
	}
    }

    PDetInsMap pdetinsmap;
    
    for (PDetSubRows::iterator dsrit = _detSubRows.begin();
	    dsrit != _detSubRows.end(); dsrit++)
    {
	for (PDetSubRow::PDetInsVector::iterator it = dsrit->GetInssVector().begin();
		it != dsrit->GetInssVector().end(); it++)
	{
	    pdetinsmap[it->GetInstance()->INSNAME] = &(*it);
	    it->SetSubRow(&(*dsrit));
	}
    }

    for (PONets::iterator nit = _nets.begin(); nit != _nets.end(); nit++)
    {
	for (PONet::PElems::iterator eit = (*nit)->GetElems().begin();
		eit != (*nit)->GetElems().end();
		eit++)
	{
	    PToPlaceIns* ptoplaceins = dynamic_cast<PToPlaceIns*>(*eit);
	    if (ptoplaceins)
	    {
		PDetInsMap::iterator iit =
		    pdetinsmap.find(ptoplaceins->GetInstance()->INSNAME);
		if (iit == pdetinsmap.end())
		{
		    cerr << "det ins " << ptoplaceins->GetInstance()->INSNAME
			 << "is not present in data... " << endl;
		    exit(1);
		}
		*eit = iit->second;
	    }
	}
    }
    //verification
    for (PONets::iterator nit = _nets.begin(); nit != _nets.end(); nit++)
    {
	for (PONet::PElems::iterator eit = (*nit)->GetElems().begin();
		eit != (*nit)->GetElems().end();
		eit++)
	{
	    assert(!dynamic_cast<PToPlaceIns*>(*eit));
	}
    }
    // on efface ce qui ne nous sert plus.
    //_rows.clear();
    //_toPlaceInss.clear();
    //_rowsYMax.clear();
    //_rowsYMinInv.clear();

    //expanding instances in subrows
    for (PDetSubRows::iterator dsrit = _detSubRows.begin();
	    dsrit != _detSubRows.end(); dsrit++)
    {
	dsrit->ExpandInstances();
    }

    // Updating All _nets BBoxs
    DetPlaceNetCost();
}

// ======================================================================
// double DetPlaceNetCost();
// ======================================================================
double
PPlacement::DetPlaceNetCost()
{
    double NetCost = 0.0;

    vector<PONet*>::iterator nfirst = _nets.begin();
    vector<PONet*>::iterator nlast  = _nets.end();
    while (nfirst != nlast) {
	PONet& net = **nfirst++;
	PBBox& bbox = net.CurrentBBox();
	vector<PElem*>::iterator ifirst = net.GetElems().begin();
	vector<PElem*>::iterator ilast  = net.GetElems().end();
	while (ifirst != ilast)
	    bbox.Merge((*ifirst++)->GetPos());

	NetCost += bbox.GetHeight() + bbox.GetWidth();
    }
    return NetCost;
}

// ======================================================================
// bool FinalOptimize()
// ======================================================================
bool
PPlacement::FinalOptimize()
{
    int  Loop = 0;
    double NewCost;
    double InitCost = DetPlaceDebugNetCost();
    _detInitNetCost = InitCost;
    if (_verbose)
	cout << " o Net Cost before Final Optimization... "
	    << InitCost << endl;
    bool ContinueCondition = true;
    bool OptimizationResult = false;
    while (ContinueCondition && (Loop < _maxDetLoop)) {
	ContinueCondition = false;

	for (PDetSubRows::iterator dsrit = _detSubRows.begin();
		dsrit != _detSubRows.end(); dsrit++)
	{
	    if (dsrit->FinalOptimize())
	    {
		ContinueCondition = true;
		OptimizationResult = true;
	    }
	    NewCost = DetPlaceDebugNetCost();
	    InitCost = NewCost;
	}
    ++Loop;
    }
    return OptimizationResult;
}

// ======================================================================
// int Save()
// ======================================================================
int
PPlacement::Save()
{
    char* phfigname;
    struct phfig* physicalfig;

    if (!_prePlaceFig)
    {
	physicalfig = addphfig(_fileName);
	phfigname = _fileName;
	if (_verbose) cout << "NO PREPLACEMENT GIVEN" << endl;
	physicalfig->XAB1 =(int)(BBox.GetMinX() * PITCH);
	physicalfig->XAB2 =(int)(BBox.GetMaxX() * PITCH);
	physicalfig->YAB1 =(int)(BBox.GetMinY() * PITCH);
	physicalfig->YAB2 =(int)(BBox.GetMaxY() * PITCH);
    }
    else
    {
	if (_verbose) cout << "PREPLACEMENT GIVEN" << endl;
	physicalfig = _prePlaceFig;
	phfigname = physicalfig->NAME;
	physicalfig->NAME = _fileName;
    }
    
    for (PDetSubRows::iterator dsrit = _detSubRows.begin();
	    dsrit != _detSubRows.end(); dsrit++)
    {
	for (PDetSubRow::PDetInsVector::iterator it = dsrit->GetInssVector().begin();
		it != dsrit->GetInssVector().end(); it++)
	{
	    it->Save(physicalfig, _dx, _dy);
	}
    }

    for (PCons::const_iterator cit = _cons.begin(); cit != _cons.end(); cit++)
    {
    	(*cit)->Save(physicalfig, _dx, _dy);
    }
    
    int superr = AddRowend(physicalfig);
    
    savephfig(physicalfig);
    physicalfig->NAME = phfigname;
    
    return superr;
}

// ======================================================================
// void AddRowend(struct phfig* physicalfig)
// ======================================================================
int PPlacement::AddRowend(struct phfig* physicalfig)
{
  int superr = 0;
  
  long xmax = (long) (BBox.GetMaxX());
  long ymax = (long) (BBox.GetMaxY() / ROWHEIGHT);
    
  // tab represents the circuit
  bool tab[xmax][ymax];
  
  for (long tabx = 0 ; tabx < xmax ; tabx++)
    for (long taby = 0 ; taby < ymax ; taby++)
      tab[tabx][taby] = false;

  // look for rowend instances already present
  // if any, we save the highest number of them
  // we will then create instances whith a bigger number
  // this prevents having two instances with the same name
  int rowendcount = 0;
  int tiecount = 0;
  for (struct phins* It = physicalfig->PHINS ; It ; It = It->NEXT)
  {
    if ( strncmp(It->INSNAME,"rowendx0_",9) == 0 )
    {
      char *nb = strdup(It->INSNAME + 9);
      if ( atoi(nb) > rowendcount ) rowendcount = atoi(nb);
    }
     
    if ( strncmp(It->INSNAME,"tiex0_",6) == 0 )
    {
      char *nb = strdup(It->INSNAME + 6);
      if ( atoi(nb) > tiecount ) tiecount = atoi(nb);
    }
     
  // fill the tab with instances (true = instance is present)
    struct phfig* insfig = getphfig(It->FIGNAME, '0');
    long width = (long) ((insfig->XAB2 - insfig->XAB1) / PITCH);
    long height = (long) ((insfig->YAB2 - insfig->YAB1) / (PITCH * ROWHEIGHT));
    long posx = (long) ((It->XINS - _dx) / PITCH);
    long posy = (long)((It->YINS - _dy)/ (PITCH * ROWHEIGHT));

    for (long ydecal=0 ; ydecal < height ; ydecal++)
    {
      for (long xdecal=0 ; xdecal < width ; xdecal++)
      {
	if (tab[posx + xdecal][posy + ydecal] == true)
	{
	  cerr << "ERROR : instances superposed : " << It->INSNAME << endl;
	  superr = 1;
	}
	tab[posx + xdecal][posy + ydecal] = true;
      }
    }
  
  }
  ++rowendcount;
  ++tiecount;
  
  for (long tabx = 0 ; tabx < xmax ; tabx++)
  {
    for (long taby = 0 ; taby < ymax ; taby++)
    { 
      if (tab[tabx][taby] == false)
      {
	// add tie cell?
	if ((tabx + 1 < xmax) && (tab[tabx+1][taby] == false))
	{
	  char buf[256];
	  char sym;
	  sprintf(buf,"tiex0_%d",rowendcount++);
	
	  if (taby%2 == 0)
	  {
	    if (_rowZeroOrientation) sym = NOSYM;
	    else			   sym = SYM_Y;
	  }
	  else // taby%2 == 1
	  {
	    if (_rowZeroOrientation) sym = SYM_Y;
	    else			   sym = NOSYM;
	  }

	  addphins(physicalfig,"tie_x0",buf,sym,(long) (tabx  * PITCH + _dx),(long) (taby * ROWHEIGHT * PITCH + _dy));
	 tab[tabx][taby]= true;
	 tab[tabx+1][taby]= true;
	} else
	  // add rowend cell
	{
	  char buf[256];
	  char sym;
	  sprintf(buf,"rowendx0_%d",rowendcount++);
	
	  if (taby%2 == 0)
	  {
	    if (_rowZeroOrientation) sym = NOSYM;
	    else			   sym = SYM_Y;
	  }
	  else // taby%2 == 1
	  {
	    if (_rowZeroOrientation) sym = SYM_Y;
	    else			   sym = NOSYM;
	  }

	  addphins(physicalfig,"rowend_x0",buf,sym,(long) (tabx  * PITCH + _dx),(long) (taby * ROWHEIGHT * PITCH + _dy));
	 tab[tabx][taby]= true;
	}

      }
    }
  }
  return superr;
}
