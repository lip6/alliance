
// -*- C++ -*-
//
// $Id: MNodes.cpp,v 1.1 2002/10/02 21:23:47 jpc Exp $
//
//  /----------------------------------------------------------------\ 
//  |                                                                |
//  |        A l l i a n c e   C A D   S y s t e m                   |
//  |              S i m p l e   R o u t e r                         |
//  |                                                                |
//  |  Author      :                    Jean-Paul CHAPUT             |
//  |  E-mail      :       alliance-support@asim.lip6.fr             |
//  | ============================================================== |
//  |  C++ Module  :       "./MNodes.cpp"                            |
//  | ************************************************************** |
//  |  U p d a t e s                                                 |
//  |                                                                |
//  \----------------------------------------------------------------/




# include  "MDefs.h"




//  /----------------------------------------------------------------\
//  |                     Methods Definitions                        |
//  \----------------------------------------------------------------/


// -------------------------------------------------------------------
// Constructor  :  "CNodeData::CNodeData()".

CNodeData::CNodeData (void)
{
  pri      = 0;

  owner    = NULL;
  rtree    = NULL;
  ident    = 0;
  obstacle = false;
  lock     = false;
}




// -------------------------------------------------------------------
// Constructor  :  "CNode::CNode()".

CNode::CNode (void)
{
  algo  = NULL;
}




// -------------------------------------------------------------------
// Method  :  "CNode::grab()".

void  CNode::grab (CNet *net, int pri, CDRGrid::iterator &coord)
    throw (bad_grab)
{
  if (data.owner && (data.owner != net))
    throw bad_grab ( data.owner->terms[getid()]->name
                   , net->name
                   , coord.x()
                   , coord.y()
                   , coord.z()
                   , pri
                   , data.pri
                   , terminal()
                   , data.ident
                   );

  // Update the net tree chaining datas.
  data.rtree = net->rtree;
  net->rtree = this;

  data.owner = net;
  data.pri   = pri;
}




// -------------------------------------------------------------------
// Method  :  "CNode::ungrab()".

void  CNode::ungrab (void)
{

  if (!terminal ()) {
    data.owner = NULL;
    data.rtree = NULL;
  }
  data.pri   = 0;
}




// -------------------------------------------------------------------
// Modifier  :  "CMatrixNodes::obstacle()".

void  CMatrixNodes::obstacle (CRect &rect, int z)
{
  CDRGrid::iterator  coord;
                int  x, y, X, Y;


  if (!z) return;

  coord = _drgrid->origin;

  X = (_drgrid->X == rect.x2) ? rect.x2 - 1 : rect.x2;
  Y = (_drgrid->Y == rect.y2) ? rect.y2 - 1 : rect.y2;

  for (x = rect.x1; x <= X; x++) {
    for (y = rect.y1; y <= Y; y++) {
      (*this)[ coord.set (x, y, z) ].data.obstacle = true;
    }
  }
}
