
// -*- C++ -*-
//
// $Id: UConst.cpp,v 1.4 2012/05/02 14:49:23 jpc Exp $
//
// +-----------------------------------------------------------------+ 
// |        A l l i a n c e   C A D   S y s t e m                    |
// |              S i m p l e   R o u t e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       alliance-support@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./UConst.cpp"                             |
// +-----------------------------------------------------------------+




# include  "UDefs.h"



// -------------------------------------------------------------------
// Namespace  :  "D".


namespace D {


  // ---------------------------------------------------------------
  // Pre-defined objects.

  const int   CHUNK_SIZE        = 4096;


  const int   cost_ALU2_X       = 2;
  const int   cost_ALU2_Y       = 6;
  const int   cost_ALU3_X       = 6;
  const int   cost_ALU3_Y       = 2;
  const int   cost_VIA          = 1;
  const bool  optim_AStar_queue = true;
  const bool  optim_AStar_order = true;
  const int   GLOBAL_HP         = 400;

  const int  ROUTING_CHOOSE     = 1;
  const int  ROUTING_GLOBAL     = 2;
  const int  ROUTING_LOCAL      = 3;


  // MBK Routing related constants.
#if 0
  const long _X_GRID             = 5;
  const long _Y_GRID             = 5;
  const long _Y_SLICE            = 50;
  const long _WIDTH_VSS          = 6;
  const long _WIDTH_VDD          = 6;
  const long _TRACK_WIDTH_ALU1   = 2;
  const long _TRACK_WIDTH_ALU2   = 2;
  const long _TRACK_WIDTH_ALU3   = 2;
  const long _TRACK_WIDTH_ALU4   = 2;
  const long _TRACK_WIDTH_ALU5   = 2;
  const long _TRACK_WIDTH_ALU6   = 2;
  const long _TRACK_WIDTH_ALU7   = 2;
  const long _TRACK_SPACING_ALU1 = 3;
  const long _TRACK_SPACING_ALU2 = 3;
  const long _TRACK_SPACING_ALU3 = 3;
  const long _TRACK_SPACING_ALU4 = 3;
  const long _TRACK_SPACING_ALU5 = 8;
  const long _TRACK_SPACING_ALU6 = 8;
  const long _TRACK_SPACING_ALU7 = 8;
#endif

  long  X_GRID;
  long  Y_GRID;
  long  WIDTH_VSS;
  long  WIDTH_VDD;
  long  Y_SLICE;
  long  TRACK_WIDTH_ALU1;
  long  TRACK_WIDTH_ALU2;
  long  TRACK_WIDTH_ALU3;
  long  TRACK_WIDTH_ALU4;
  long  TRACK_WIDTH_ALU5;
  long  TRACK_WIDTH_ALU6;
  long  TRACK_WIDTH_ALU7;
  long  TRACK_WIDTH_ALU8;
  long  TRACK_SPACING_ALU1;
  long  TRACK_SPACING_ALU2;
  long  TRACK_SPACING_ALU3;
  long  TRACK_SPACING_ALU4;
  long  TRACK_SPACING_ALU5;
  long  TRACK_SPACING_ALU6;
  long  TRACK_SPACING_ALU7;
  long  TRACK_SPACING_ALU8;




// -------------------------------------------------------------------
// End of namespace "D".

}
