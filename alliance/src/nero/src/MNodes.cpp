
// -*- C++ -*-
//
// $Id: MNodes.cpp,v 1.4 2005/10/17 23:11:06 jpc Exp $
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
               long  x, y, X, Y;


  if ( !z || (z >= _drgrid->Z) ) return;

  coord = _drgrid->origin;

  X = (_drgrid->X <= rect.x2) ? _drgrid->X - 1 : rect.x2;
  Y = (_drgrid->Y <= rect.y2) ? _drgrid->Y - 1 : rect.y2;

  x =  (rect.x1>0) ? rect.x1 : 0;
  for ( ; x <= X; x++) {
    y =  (rect.y1>0) ? rect.y1 : 0;
    for ( ; y <= Y; y++) {
      if ( ! (*this)[ coord.set (x, y, z) ].terminal () ) {
        //cdebug << "+      Obstacle at (" << x << "," << y << "," << z << ")" << "\n";
        (*this)[ coord ].data.obstacle = true;
      }
    }
  }
}
